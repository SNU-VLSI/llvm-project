#include "common_decl.h"

void test_redundant_jmp() {
  for (int i = 0; i < 4; i++) {
    short16 a = __builtin_IMCE_RECV(0);
    __builtin_IMCE_SEND(0, a, 0, 0);
  }
}
