#include "common_decl.h"

void test_zero_reg() {
  short16 var1;
  var1 = __builtin_IMCE_SUBI(0, 1);
  __builtin_IMCE_SEND(1, var1, 2, 0);
  __builtin_IMCE_STOP();
}