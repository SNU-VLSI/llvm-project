// #include "llvm/ADT/ArrayRef.h"
// #include "llvm/ADT/STLExtras.h"
// #include "llvm/ADT/ScopeExit.h"
// #include "llvm/Analysis/TargetLibraryInfo.h"
// #include "llvm/CodeGen/CallingConvLower.h"
// #include "llvm/CodeGen/LinkAllAsmWriterComponents.h"
// #include "llvm/CodeGen/LinkAllCodegenComponents.h"
// #include "llvm/CodeGen/MIRParser/MIRParser.h"
// #include "llvm/CodeGen/MachineFunctionPass.h"
// #include "llvm/CodeGen/MachineModuleInfo.h"
// #include "llvm/CodeGen/MachineRegisterInfo.h"
// #include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
// #include "llvm/CodeGen/TargetPassConfig.h"
// #include "llvm/CodeGen/TargetSubtargetInfo.h"
// #include "llvm/IR/AutoUpgrade.h"
// #include "llvm/IR/DataLayout.h"
// #include "llvm/IR/DiagnosticInfo.h"
// #include "llvm/IR/DiagnosticPrinter.h"
// #include "llvm/IR/LLVMRemarkStreamer.h"
// #include "llvm/IR/LegacyPassManager.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IR/Verifier.h"
// #include "llvm/IRReader/IRReader.h"
// #include "llvm/InitializePasses.h"
// #include "llvm/MC/MCTargetOptionsCommandFlags.h"
// #include "llvm/Pass.h"
// #include "llvm/Support/CodeGen.h"
// #include "llvm/Support/Debug.h"
// #include "llvm/Support/FileSystem.h"
// #include "llvm/Support/FormattedStream.h"
// #include "llvm/Support/PluginLoader.h"
// #include "llvm/Support/SourceMgr.h"
// #include "llvm/Support/TimeProfiler.h"
// #include "llvm/Support/ToolOutputFile.h"
// #include "llvm/Target/TargetLoweringObjectFile.h"
// #include "llvm/Transforms/Utils/Cloning.h"
// #include "llvm/Remarks/HotnessThresholdParser.h"
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>

// -----
#include "llvm/CodeGen/CommandFlags.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/WithColor.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"
#include "llvm/TargetParser/SubtargetFeature.h"
#include "llvm/TargetParser/Triple.h"

using namespace llvm;
static codegen::RegisterCodeGenFlags CGF;

// General options for llc.  Other pass-specific options are specified
// within the corresponding llc passes, and target-specific options
// and back-end code generation options are specified with the target machine.
//
static cl::opt<std::string> InputFilename(cl::Positional, cl::desc("<input bitcode>"),
                                          cl::init("-"));

static cl::opt<std::string> OutputFilename("o", cl::desc("Output filename"),
                                           cl::value_desc("filename"));

// Determine optimization level.
static cl::opt<char> OptLevel("O",
                              cl::desc("Optimization level. [-O0, -O1, -O2, or -O3] "
                                       "(default = '-O2')"),
                              cl::Prefix, cl::init('2'));

static cl::opt<std::string> TargetTriple("mtriple", cl::desc("Override target triple for module"));

static int compileModule(char **, LLVMContext &);

// main - Entry point for the llc compiler.
//
int main(int argc, char **argv) {
  InitLLVM X(argc, argv);

  // Enable debug stream buffering.
  // bool EnableDebugBuffering = true;

  // Initialize targets first, so that --version shows registered targets.
  LLVMInitializeRISCVAsmParser();
  LLVMInitializeRISCVAsmPrinter();
  LLVMInitializeRISCVTarget();
  LLVMInitializeRISCVTargetMC();
  LLVMInitializeRISCVTargetMCA();
  LLVMInitializeRISCVTargetInfo();
  LLVMInitializeRISCVDisassembler();
  // InitializeAllTargets();
  // InitializeAllTargetMCs();
  // InitializeAllAsmPrinters();
  // InitializeAllAsmParsers();

  // Register the Target and CPU printer for --version.
  // cl::AddExtraVersionPrinter(sys::printDefaultTargetAndDetectedCPU);
  // Register the target printer for --version.
  // cl::AddExtraVersionPrinter(TargetRegistry::printRegisteredTargetsForVersion);

  cl::ParseCommandLineOptions(argc, argv, "llvm system compiler\n");

  LLVMContext Context;
  // Context.setDiscardValueNames(DiscardValueNames);

  // Set a diagnostic handler that doesn't exit on the first error
  // Compile the module TimeCompilations times to give better compile time
  // metrics.
  int RetVal = compileModule(argv, Context);
  return 0;
}

static int compileModule(char **argv, LLVMContext &Context) {
  // Load the module to be compiled...
  std::unique_ptr<Module> M;
  Triple TheTriple;
  std::string CPUStr = codegen::getCPUStr(), FeaturesStr = codegen::getFeaturesStr();
  CodeGenOpt::Level OLvl = CodeGenOpt::None;
  TargetOptions Options;
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

  Target = std::unique_ptr<TargetMachine>(TheTarget->createTargetMachine(
      TheTriple.getTriple(), CPUStr, FeaturesStr, Options, RM, CM, OLvl));
  assert(Target && "Could not allocate target machine!");

  SelectionDAG DAG(*Target, OLvl);

  printf("Hello, world!\n");

  // If we don't have a module then just exit now. We do this down
  // here since the CPU/Feature help is underneath the target machine
  // creation.
  return 0;
}
