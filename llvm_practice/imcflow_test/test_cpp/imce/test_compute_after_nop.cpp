#include "common_decl.h"

// Test case to verify computation after NOP instructions
// Tests batch normalization operations following NOPs
void test_compute_after_nop() {
  __builtin_IMCE_NOP();
  __builtin_IMCE_NOP();

  short16 var1 = __builtin_IMCE_GET_CREG((short)0);
  short16 var2 = __builtin_IMCE_GET_CREG((short)1);
  short16 var3 = __builtin_IMCE_GET_CREG((short)2);
  short16 var4 = __builtin_IMCE_GET_CREG((short)3);

  __builtin_IMCE_SEND(1, var1, 2, 0);
  __builtin_IMCE_SEND(1, var2, 2, 0);
  __builtin_IMCE_SEND(1, var3, 2, 0);
  __builtin_IMCE_SEND(1, var4, 2, 0);
}
