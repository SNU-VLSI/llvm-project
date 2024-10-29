#ifndef LLVM_TRANSFORMS_INODE_CORE_ID_ASSIGN_WRAP_H
#define LLVM_TRANSFORMS_INODE_CORE_ID_ASSIGN_WRAP_H

#include "llvm/IR/PassManager.h"
namespace llvm {
class Function;

/// This pass performs function-level constant propagation and merging.
class INODECoreIDAssignWrapPass : public PassInfoMixin<INODECoreIDAssignWrapPass> {
public:
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // end namespace llvm

#endif