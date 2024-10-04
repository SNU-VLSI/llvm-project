#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_IMCETOOLCHAIN_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_IMCETOOLCHAIN_H

#include "Gnu.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {
namespace toolchains {

class LLVM_LIBRARY_VISIBILITY IMCEToolChain : public Generic_ELF {
public:
  IMCEToolChain(const Driver &D, const llvm::Triple &Triple, const llvm::opt::ArgList &Args);
};
}
}
}

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_IMCETOOLCHAIN_H