#include "IMCEToolchain.h"
#include "CommonArgs.h"
#include "clang/Driver/Compilation.h"
#include "clang/Driver/InputInfo.h"
#include "clang/Driver/Options.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang::driver;
using namespace clang::driver::toolchains;
using namespace clang::driver::tools;
using namespace clang;
using namespace llvm::opt;

IMCEToolChain::IMCEToolChain(const Driver &D, const llvm::Triple &Triple,
                               const ArgList &Args)
    : Generic_ELF(D, Triple, Args) {
  // GCCInstallation.init(Triple, Args);
  // if (GCCInstallation.isValid()) {
  //   Multilibs = GCCInstallation.getMultilibs();
  //   SelectedMultilibs.assign({GCCInstallation.getMultilib()});
  //   path_list &Paths = getFilePaths();
  //   // Add toolchain/multilib specific file paths.
  //   addMultilibsFilePaths(D, Multilibs, SelectedMultilibs.back(),
  //                         GCCInstallation.getInstallPath(), Paths);
  //   getFilePaths().push_back(GCCInstallation.getInstallPath().str());
  //   ToolChain::path_list &PPaths = getProgramPaths();
  //   // Multilib cross-compiler GCC installations put ld in a triple-prefixed
  //   // directory off of the parent of the GCC installation.
  //   PPaths.push_back(Twine(GCCInstallation.getParentLibPath() + "/../" +
  //                          GCCInstallation.getTriple().str() + "/bin")
  //                        .str());
  //   PPaths.push_back((GCCInstallation.getParentLibPath() + "/../bin").str());
  // } else {
  //   getProgramPaths().push_back(D.Dir);
  // }
  // getFilePaths().push_back(computeSysRoot() + "/lib");
}