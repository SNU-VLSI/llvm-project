add_llvm_component_group(IMCE)

set(LLVM_TARGET_DEFINITIONS IMCE.td)

tablegen(LLVM IMCEGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM IMCEGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM IMCEGenCallingConv.inc -gen-callingconv)
tablegen(LLVM IMCEGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM IMCEGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM IMCEGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM IMCEGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM IMCEGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM IMCEGenSubtargetInfo.inc -gen-subtarget)

tablegen(LLVM IMCEGenRegisterBank.inc -gen-register-bank)

add_public_tablegen_target(IMCECommonTableGen)

add_llvm_target(IMCECodeGen
  IMCEAsmPrinter.cpp
  IMCEFrameLowering.cpp
  IMCEInstrInfo.cpp
  IMCEISelDAGToDAG.cpp
  IMCEISelLowering.cpp
  IMCEMCInstLower.cpp
  IMCERegisterInfo.cpp
  IMCESubtarget.cpp
  IMCETargetMachine.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  Scalar
  SelectionDAG
  Support
  IMCEDesc
  IMCEInfo
  Target

  ADD_TO_COMPONENT
  IMCE
)

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
