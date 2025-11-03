#include "common_decl.h"

void long_branch() {
  for(int i=0; i<16; i++) {
    for(int j=0; j<4; j++) {
      short16 a = __builtin_IMCE_RECV(0);
      short16 b = __builtin_IMCE_RECV(1);
      short16 result = __builtin_IMCE_ADD(a, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      result = __builtin_IMCE_ADD(result, b, 0);
      short16 relu_result = __builtin_IMCE_MAX(result, 0, 0);
      __builtin_IMCE_SEND(0, relu_result, 0, 0);
    }
  }
}