#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/ScopeExit.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/LinkAllAsmWriterComponents.h"
#include "llvm/CodeGen/LinkAllCodegenComponents.h"
#include "llvm/CodeGen/MIRParser/MIRParser.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/AutoUpgrade.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DiagnosticPrinter.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LLVMRemarkStreamer.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/InitializePasses.h"
#include "llvm/MC/MCTargetOptionsCommandFlags.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Pass.h"
#include "llvm/Remarks/HotnessThresholdParser.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/PluginLoader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TimeProfiler.h"
#include "llvm/Support/ToolOutputFile.h"
#include "llvm/Support/WithColor.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/TargetParser/SubtargetFeature.h"
#include "llvm/TargetParser/Triple.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>

// -----
#include "llvm/CodeGen/CommandFlags.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/TargetSelect.h"

using namespace llvm;

static codegen::RegisterCodeGenFlags CGF;

// General options for llc.  Other pass-specific options are specified
// within the corresponding llc passes, and target-specific options
// and back-end code generation options are specified with the target machine.
//
static cl::opt<std::string> InputFilename(cl::Positional, cl::desc("<input bitcode>"),
                                          cl::init("-"));

static cl::opt<std::string> InputLanguage("x", cl::desc("Input language ('ir' or 'mir')"));

static cl::opt<std::string> OutputFilename("o", cl::desc("Output filename"),
                                           cl::value_desc("filename"));

static cl::opt<std::string> SplitDwarfOutputFile("split-dwarf-output",
                                                 cl::desc(".dwo output filename"),
                                                 cl::value_desc("filename"));

static cl::opt<unsigned> TimeCompilations("time-compilations", cl::Hidden, cl::init(1u),
                                          cl::value_desc("N"),
                                          cl::desc("Repeat compilation N times for timing"));

static cl::opt<bool> TimeTrace("time-trace", cl::desc("Record time trace"));

static cl::opt<unsigned> TimeTraceGranularity(
    "time-trace-granularity",
    cl::desc("Minimum time granularity (in microseconds) traced by time profiler"), cl::init(500),
    cl::Hidden);

static cl::opt<std::string> TimeTraceFile("time-trace-file",
                                          cl::desc("Specify time trace file destination"),
                                          cl::value_desc("filename"));

static cl::opt<std::string>
    BinutilsVersion("binutils-version", cl::Hidden,
                    cl::desc("Produced object files can use all ELF features "
                             "supported by this binutils version and newer."
                             "If -no-integrated-as is specified, the generated "
                             "assembly will consider GNU as support."
                             "'none' means that all ELF features can be used, "
                             "regardless of binutils support"));

static cl::opt<bool> PreserveComments("preserve-as-comments", cl::Hidden,
                                      cl::desc("Preserve Comments in outputted assembly"),
                                      cl::init(true));

// Determine optimization level.
static cl::opt<char> OptLevel("O",
                              cl::desc("Optimization level. [-O0, -O1, -O2, or -O3] "
                                       "(default = '-O2')"),
                              cl::Prefix, cl::init('2'));

static cl::opt<std::string> TargetTriple("mtriple", cl::desc("Override target triple for module"));

static cl::opt<std::string>
    SplitDwarfFile("split-dwarf-file",
                   cl::desc("Specify the name of the .dwo file to encode in the DWARF output"));

static cl::opt<bool> NoVerify("disable-verify", cl::Hidden, cl::desc("Do not verify input module"));

static cl::opt<bool> DisableSimplifyLibCalls("disable-simplify-libcalls",
                                             cl::desc("Disable simplify-libcalls"));

static cl::opt<bool> ShowMCEncoding("show-mc-encoding", cl::Hidden,
                                    cl::desc("Show encoding in .s output"));

static cl::opt<bool> DwarfDirectory("dwarf-directory", cl::Hidden,
                                    cl::desc("Use .file directives with an explicit directory"),
                                    cl::init(true));

static cl::opt<bool> AsmVerbose("asm-verbose", cl::desc("Add comments to directives."),
                                cl::init(true));

static cl::opt<bool> CompileTwice("compile-twice", cl::Hidden,
                                  cl::desc("Run everything twice, re-using the same pass "
                                           "manager and verify the result is the same."),
                                  cl::init(false));

static cl::opt<bool>
    DiscardValueNames("discard-value-names",
                      cl::desc("Discard names from Value (other than GlobalValue)."),
                      cl::init(false), cl::Hidden);

static cl::list<std::string> IncludeDirs("I", cl::desc("include search path"));

static cl::opt<bool>
    RemarksWithHotness("pass-remarks-with-hotness",
                       cl::desc("With PGO, include profile count in optimization remarks"),
                       cl::Hidden);

static cl::opt<std::optional<uint64_t>, false, remarks::HotnessThresholdParser>
    RemarksHotnessThreshold("pass-remarks-hotness-threshold",
                            cl::desc("Minimum profile count required for "
                                     "an optimization remark to be output. "
                                     "Use 'auto' to apply the threshold from profile summary."),
                            cl::value_desc("N or 'auto'"), cl::init(0), cl::Hidden);

static cl::opt<std::string> RemarksFilename("pass-remarks-output",
                                            cl::desc("Output filename for pass remarks"),
                                            cl::value_desc("filename"));

static cl::opt<std::string>
    RemarksPasses("pass-remarks-filter",
                  cl::desc("Only record optimization remarks from passes whose "
                           "names match the given regular expression"),
                  cl::value_desc("regex"));

static cl::opt<std::string>
    RemarksFormat("pass-remarks-format",
                  cl::desc("The format used for serializing remarks (default: YAML)"),
                  cl::value_desc("format"), cl::init("yaml"));

namespace {

struct RunPassOption {
  void operator=(const std::string &Val) const {}
};
} // namespace

static RunPassOption RunPassOpt;

static cl::opt<RunPassOption, true, cl::parser<std::string>>
    RunPass("run-pass", cl::desc("Run compiler only for specified passes (comma separated list)"),
            cl::value_desc("pass-name"), cl::location(RunPassOpt));

static int compileModule(char **, LLVMContext &);

[[noreturn]] static void reportError(Twine Msg, StringRef Filename = "") {
  SmallString<256> Prefix;
  if (!Filename.empty()) {
    if (Filename == "-")
      Filename = "<stdin>";
    ("'" + Twine(Filename) + "': ").toStringRef(Prefix);
  }
  WithColor::error(errs(), "llc") << Prefix << Msg << "\n";
  exit(1);
}

[[noreturn]] static void reportError(Error Err, StringRef Filename) {
  assert(Err);
  handleAllErrors(createFileError(Filename, std::move(Err)),
                  [&](const ErrorInfoBase &EI) { reportError(EI.message()); });
  llvm_unreachable("reportError() should not return");
}

struct LLCDiagnosticHandler : public DiagnosticHandler {
  bool *HasError;
  LLCDiagnosticHandler(bool *HasErrorPtr) : HasError(HasErrorPtr) {}
  bool handleDiagnostics(const DiagnosticInfo &DI) override {
    if (DI.getKind() == llvm::DK_SrcMgr) {
      const auto &DISM = cast<DiagnosticInfoSrcMgr>(DI);
      const SMDiagnostic &SMD = DISM.getSMDiag();

      if (SMD.getKind() == SourceMgr::DK_Error)
        *HasError = true;

      SMD.print(nullptr, errs());

      // For testing purposes, we print the LocCookie here.
      if (DISM.isInlineAsmDiag() && DISM.getLocCookie())
        WithColor::note() << "!srcloc = " << DISM.getLocCookie() << "\n";

      return true;
    }

    if (DI.getSeverity() == DS_Error)
      *HasError = true;

    if (auto *Remark = dyn_cast<DiagnosticInfoOptimizationBase>(&DI))
      if (!Remark->isEnabled())
        return true;

    DiagnosticPrinterRawOStream DP(errs());
    errs() << LLVMContext::getDiagnosticMessagePrefix(DI.getSeverity()) << ": ";
    DI.print(DP);
    errs() << "\n";
    return true;
  }
};

// main - Entry point for the llc compiler.
//
int main(int argc, char **argv) {
  InitLLVM X(argc, argv);

  // Enable debug stream buffering.
  EnableDebugBuffering = true;

  // Initialize targets first, so that --version shows registered targets.
  InitializeAllTargets();
  InitializeAllTargetMCs();
  InitializeAllAsmPrinters();
  InitializeAllAsmParsers();

  // Initialize codegen and IR passes used by llc so that the -print-after,
  // -print-before, and -stop-after options work.
  // PassRegistry *Registry = PassRegistry::getPassRegistry();
  // initializeCore(*Registry);
  // initializeCodeGen(*Registry);
  // initializeLoopStrengthReducePass(*Registry);
  // initializeLowerIntrinsicsPass(*Registry);
  // initializeUnreachableBlockElimLegacyPassPass(*Registry);
  // initializeConstantHoistingLegacyPassPass(*Registry);
  // initializeScalarOpts(*Registry);
  // initializeVectorization(*Registry);
  // initializeScalarizeMaskedMemIntrinLegacyPassPass(*Registry);
  // initializeExpandReductionsPass(*Registry);
  // initializeExpandVectorPredicationPass(*Registry);
  // initializeHardwareLoopsLegacyPass(*Registry);
  // initializeTransformUtils(*Registry);
  // initializeReplaceWithVeclibLegacyPass(*Registry);
  // initializeTLSVariableHoistLegacyPassPass(*Registry);

  // // Initialize debugging passes.
  // initializeScavengerTestPass(*Registry);

  // Register the Target and CPU printer for --version.
  cl::AddExtraVersionPrinter(sys::printDefaultTargetAndDetectedCPU);
  // Register the target printer for --version.
  cl::AddExtraVersionPrinter(TargetRegistry::printRegisteredTargetsForVersion);

  cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");

  if (TimeTrace)
    timeTraceProfilerInitialize(TimeTraceGranularity, argv[0]);
  auto TimeTraceScopeExit = make_scope_exit([]() {
    if (TimeTrace) {
      if (auto E = timeTraceProfilerWrite(TimeTraceFile, OutputFilename)) {
        handleAllErrors(std::move(E),
                        [&](const StringError &SE) { errs() << SE.getMessage() << "\n"; });
        return;
      }
      timeTraceProfilerCleanup();
    }
  });

  LLVMContext Context;
  Context.setDiscardValueNames(DiscardValueNames);

  // Set a diagnostic handler that doesn't exit on the first error
  bool HasError = false;
  Context.setDiagnosticHandler(std::make_unique<LLCDiagnosticHandler>(&HasError));

  Expected<std::unique_ptr<ToolOutputFile>> RemarksFileOrErr =
      setupLLVMOptimizationRemarks(Context, RemarksFilename, RemarksPasses, RemarksFormat,
                                   RemarksWithHotness, RemarksHotnessThreshold);
  if (Error E = RemarksFileOrErr.takeError())
    reportError(std::move(E), RemarksFilename);
  std::unique_ptr<ToolOutputFile> RemarksFile = std::move(*RemarksFileOrErr);

  if (InputLanguage != "" && InputLanguage != "ir" && InputLanguage != "mir")
    reportError("input language must be '', 'IR' or 'MIR'");

  // Compile the module TimeCompilations times to give better compile time
  // metrics.
  for (unsigned I = TimeCompilations; I; --I)
    if (int RetVal = compileModule(argv, Context))
      return RetVal;

  if (RemarksFile)
    RemarksFile->keep();
  return 0;
}

static int compileModule(char **argv, LLVMContext &Context) {
  // Load the module to be compiled...
  SMDiagnostic Err;
  std::unique_ptr<Module> M;
  std::unique_ptr<MIRParser> MIR;
  Triple TheTriple;
  std::string CPUStr = codegen::getCPUStr(), FeaturesStr = codegen::getFeaturesStr();

  // Set attributes on functions as loaded from MIR from command line arguments.
  auto setMIRFunctionAttributes = [&CPUStr, &FeaturesStr](Function &F) {
    codegen::setFunctionAttributes(CPUStr, FeaturesStr, F);
  };

  auto MAttrs = codegen::getMAttrs();
  bool SkipModule = CPUStr == "help" || (!MAttrs.empty() && MAttrs.front() == "help");

  CodeGenOpt::Level OLvl;
  if (auto Level = CodeGenOpt::parseLevel(OptLevel)) {
    OLvl = *Level;
  } else {
    WithColor::error(errs(), argv[0]) << "invalid optimization level.\n";
    return 1;
  }

  // Parse 'none' or '$major.$minor'. Disallow -binutils-version=0 because we
  // use that to indicate the MC default.
  if (!BinutilsVersion.empty() && BinutilsVersion != "none") {
    StringRef V = BinutilsVersion.getValue();
    unsigned Num;
    if (V.consumeInteger(10, Num) || Num == 0 ||
        !(V.empty() || (V.consume_front(".") && !V.consumeInteger(10, Num) && V.empty()))) {
      WithColor::error(errs(), argv[0])
          << "invalid -binutils-version, accepting 'none' or major.minor\n";
      return 1;
    }
  }
  TargetOptions Options;
  auto InitializeOptions = [&](const Triple &TheTriple) {
    Options = codegen::InitTargetOptionsFromCodeGenFlags(TheTriple);

    if (Options.XCOFFReadOnlyPointers) {
      if (!TheTriple.isOSAIX())
        reportError("-mxcoff-roptr option is only supported on AIX", InputFilename);

      // Since the storage mapping class is specified per csect,
      // without using data sections, it is less effective to use read-only
      // pointers. Using read-only pointers may cause other RO variables in the
      // same csect to become RW when the linker acts upon `-bforceimprw`;
      // therefore, we require that separate data sections are used in the
      // presence of ReadOnlyPointers. We respect the setting of data-sections
      // since we have not found reasons to do otherwise that overcome the user
      // surprise of not respecting the setting.
      if (!Options.DataSections)
        reportError("-mxcoff-roptr option must be used with -data-sections", InputFilename);
    }

    Options.BinutilsVersion = TargetMachine::parseBinutilsVersion(BinutilsVersion);
    Options.MCOptions.ShowMCEncoding = ShowMCEncoding;
    Options.MCOptions.AsmVerbose = AsmVerbose;
    Options.MCOptions.PreserveAsmComments = PreserveComments;
    Options.MCOptions.IASSearchPaths = IncludeDirs;
    Options.MCOptions.SplitDwarfFile = SplitDwarfFile;
    if (DwarfDirectory.getPosition()) {
      Options.MCOptions.MCUseDwarfDirectory = DwarfDirectory
                                                  ? MCTargetOptions::EnableDwarfDirectory
                                                  : MCTargetOptions::DisableDwarfDirectory;
    } else {
      // -dwarf-directory is not set explicitly. Some assemblers
      // (e.g. GNU as or ptxas) do not support `.file directory'
      // syntax prior to DWARFv5. Let the target decide the default
      // value.
      Options.MCOptions.MCUseDwarfDirectory = MCTargetOptions::DefaultDwarfDirectory;
    }
  };

  std::optional<Reloc::Model> RM = codegen::getExplicitRelocModel();
  std::optional<CodeModel::Model> CM = codegen::getExplicitCodeModel();

  const Target *TheTarget = nullptr;
  std::unique_ptr<TargetMachine> Target;

  TheTriple = Triple(Triple::normalize(TargetTriple));
  if (TheTriple.getTriple().empty())
    TheTriple.setTriple(sys::getDefaultTargetTriple());

  // Get the target specific parser.
  std::string Error;
  TheTarget = TargetRegistry::lookupTarget(codegen::getMArch(), TheTriple, Error);
  if (!TheTarget) {
    WithColor::error(errs(), argv[0]) << Error;
    return 1;
  }

  // On AIX, setting the relocation model to anything other than PIC is
  // considered a user error.
  if (TheTriple.isOSAIX() && RM && *RM != Reloc::PIC_) {
    WithColor::error(errs(), argv[0]) << "invalid relocation model, AIX only supports PIC.\n";
    return 1;
  }

  InitializeOptions(TheTriple);
  Target = std::unique_ptr<TargetMachine>(TheTarget->createTargetMachine(
      TheTriple.getTriple(), CPUStr, FeaturesStr, Options, RM, CM, OLvl));
  assert(Target && "Could not allocate target machine!");

  SelectionDAG DAG(*Target, OLvl);

  // If we don't have a module then just exit now. We do this down
  // here since the CPU/Feature help is underneath the target machine
  // creation.
  return 0;
}
