add_llvm_component_group(INODE)

set(LLVM_TARGET_DEFINITIONS INODE.td)

tablegen(LLVM INODEGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM INODEGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM INODEGenCallingConv.inc -gen-callingconv)
tablegen(LLVM INODEGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM INODEGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM INODEGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM INODEGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM INODEGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM INODEGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(INODECommonTableGen)

add_llvm_target(INODECodeGen
  INODEAsmPrinter.cpp
  INODEFrameLowering.cpp
  INODEInstrInfo.cpp
  INODEISelDAGToDAG.cpp
  INODEISelLowering.cpp
  INODEMCInstLower.cpp
  INODERegisterInfo.cpp
  INODESubtarget.cpp
  INODETargetMachine.cpp
  INODETargetTransformInfo.cpp
  INODEIRPasses.cpp
  INODEMIRPasses.cpp
  INODECountedLoopMIRPass.cpp
  INODECountedLoopOptions.cpp
  INODECoreIDAssign.cpp
  SCCPLegacy.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  Scalar
  SelectionDAG
  Support
  INODEDesc
  INODEInfo
  Target

  ADD_TO_COMPONENT
  INODE
)

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
