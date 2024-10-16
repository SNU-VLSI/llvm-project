#include "common_decl.h"

void relu() {
  for(short i=0; i<16; i++) {
    short16 result = __builtin_IMCE_RECV(0);
    short16 relu_result = __builtin_IMCE_MAX(result, 0, 0);
    __builtin_IMCE_SEND(0, relu_result, 0, 0);
  }
}