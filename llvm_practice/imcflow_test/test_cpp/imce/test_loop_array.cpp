#include "common_decl.h"

void test_loop_array() {  
  short16 var1;
  short16 var2;
  short16 var3;
  short16 var4;
  short16 var5;
  short16 var6;
  short16 var7;
  short16 var8;
  short16 var9;
  short16 var10; 
  short16 var11; 
  short16 var12; 
  short16 arr1[8]; // array to hold intermediate results
  short16 arr2[8]; // array to hold intermediate results
  short16 arr3[8]; // array to hold intermediate results
  short16 arr4[8]; // array to hold intermediate results

  var5 = __builtin_IMCE_RECV(1);
  var6 = __builtin_IMCE_RECV(1);

  for (int i1 = 0; i1 < 8; i1++) {
    __builtin_IMCE_NOP();
    __builtin_IMCE_NOP();

    var1 = __builtin_IMCE_GET_CREG((short)0);
    var2 = __builtin_IMCE_GET_CREG((short)1);
    var3 = __builtin_IMCE_GET_CREG((short)2);
    var4 = __builtin_IMCE_GET_CREG((short)3);

    arr1[i1] = __builtin_IMCE_MULTL(var1, var5, 15);
    arr1[i1] = __builtin_IMCE_ADD(arr1[i1], var6, 15);
    arr2[i1] = __builtin_IMCE_MULTL(var2, var7, 15);
    arr2[i1] = __builtin_IMCE_ADD(arr2[i1], var8, 15);
    arr3[i1] = __builtin_IMCE_MULTL(var3, var9, 15);
    arr3[i1] = __builtin_IMCE_ADD(arr3[i1], var10, 15);
    arr4[i1] = __builtin_IMCE_MULTL(var4, var11, 15);
    arr4[i1] = __builtin_IMCE_ADD(arr4[i1], var12, 15);
  }

  for (int i1 = 0; i1 < 8; i1++) {
    __builtin_IMCE_SEND(1, arr1[i1], 2, 0);
    __builtin_IMCE_SEND(1, arr2[i1], 2, 0);
    __builtin_IMCE_SEND(1, arr3[i1], 2, 0);
    __builtin_IMCE_SEND(1, arr4[i1], 2, 0);
  }

  __builtin_IMCE_STOP();
}