#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_INODETOOLCHAIN_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_INODETOOLCHAIN_H

#include "Gnu.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {
namespace toolchains {

class LLVM_LIBRARY_VISIBILITY INODEToolChain : public Generic_ELF {
public:
  INODEToolChain(const Driver &D, const llvm::Triple &Triple, const llvm::opt::ArgList &Args);
};
}
}
}

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_INODETOOLCHAIN_H