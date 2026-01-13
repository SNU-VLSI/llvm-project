#include "common_decl.h"

// Test case to verify NOP insertion between vaddi and standby
// Expected output should be: vaddi, nop, standby
short16 test_standby_nop_insertion(short16 a) {
  // This should trigger the hardware bug workaround
  // The second parameter is an immediate, so LLVM will use vaddi to materialize it
  // The pass should detect this and insert a NOP
  __builtin_IMCE_STANDBY(1, 2);

  return a;
}
