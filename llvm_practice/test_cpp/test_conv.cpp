#include "common_decl.h"
void conv2d() {
  short var_24 = 0;
  for (int i0 = 0; i0 < 66; i0++) { // load_loop_start_0_0
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_LOAD_LB(0);
  }
  __builtin_IMCE_STEP();
  short16 var_25 = __builtin_IMCE_GET_CREG((short)0);
  short16 var_26 = __builtin_IMCE_GET_CREG((short)1);
  short16 var_27 = __builtin_IMCE_GET_CREG((short)2);
  short16 var_28 = __builtin_IMCE_GET_CREG((short)3);
  short16 var_29 = __builtin_IMCE_RECV(5);
   var_29 = __builtin_IMCE_ADD(var_25, var_29, 15);
  __builtin_IMCE_SEND(3, var_29, 2, 0);
  short16 var_30 = __builtin_IMCE_RECV(5);
   var_30 = __builtin_IMCE_ADD(var_26, var_30, 15);
  __builtin_IMCE_SEND(3, var_30, 2, 0);
  short16 var_31 = __builtin_IMCE_RECV(5);
   var_31 = __builtin_IMCE_ADD(var_27, var_31, 15);
  __builtin_IMCE_SEND(3, var_31, 2, 0);
  short16 var_32 = __builtin_IMCE_RECV(5);
   var_32 = __builtin_IMCE_ADD(var_28, var_32, 15);
  __builtin_IMCE_SEND(3, var_32, 2, 0);
   var_24 = var_24 + (short)1;
  __builtin_IMCE_SETFLAG(var_24);
  for (int i1 = 0; i1 < 28; i1++) { // sliding_loop_start_0_1
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_LOAD_LB(0);
    __builtin_IMCE_STEP();
    short16 var_33 = __builtin_IMCE_GET_CREG((short)0);
    short16 var_34 = __builtin_IMCE_GET_CREG((short)1);
    short16 var_35 = __builtin_IMCE_GET_CREG((short)2);
    short16 var_36 = __builtin_IMCE_GET_CREG((short)3);
    short16 var_37 = __builtin_IMCE_RECV(5);
     var_37 = __builtin_IMCE_ADD(var_33, var_37, 15);
    __builtin_IMCE_SEND(3, var_37, 2, 0);
    short16 var_38 = __builtin_IMCE_RECV(5);
     var_38 = __builtin_IMCE_ADD(var_34, var_38, 15);
    __builtin_IMCE_SEND(3, var_38, 2, 0);
    short16 var_39 = __builtin_IMCE_RECV(5);
     var_39 = __builtin_IMCE_ADD(var_35, var_39, 15);
    __builtin_IMCE_SEND(3, var_39, 2, 0);
    short16 var_40 = __builtin_IMCE_RECV(5);
     var_40 = __builtin_IMCE_ADD(var_36, var_40, 15);
    __builtin_IMCE_SEND(3, var_40, 2, 0);
     var_24 = var_24 + (short)1;
    __builtin_IMCE_SETFLAG(var_24);
  }
  for (int i2 = 0; i2 < 28; i2++) { // region_loop_start_region_1
    for (int i3 = 0; i3 < 2; i3++) { // load_loop_start_1_0
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_LOAD_LB(0);
    }
    __builtin_IMCE_STEP();
    short16 var_41 = __builtin_IMCE_GET_CREG((short)0);
    short16 var_42 = __builtin_IMCE_GET_CREG((short)1);
    short16 var_43 = __builtin_IMCE_GET_CREG((short)2);
    short16 var_44 = __builtin_IMCE_GET_CREG((short)3);
    short16 var_45 = __builtin_IMCE_RECV(5);
     var_45 = __builtin_IMCE_ADD(var_41, var_45, 15);
    __builtin_IMCE_SEND(3, var_45, 2, 0);
    short16 var_46 = __builtin_IMCE_RECV(5);
     var_46 = __builtin_IMCE_ADD(var_42, var_46, 15);
    __builtin_IMCE_SEND(3, var_46, 2, 0);
    short16 var_47 = __builtin_IMCE_RECV(5);
     var_47 = __builtin_IMCE_ADD(var_43, var_47, 15);
    __builtin_IMCE_SEND(3, var_47, 2, 0);
    short16 var_48 = __builtin_IMCE_RECV(5);
     var_48 = __builtin_IMCE_ADD(var_44, var_48, 15);
    __builtin_IMCE_SEND(3, var_48, 2, 0);
     var_24 = var_24 + (short)1;
    __builtin_IMCE_SETFLAG(var_24);
    for (int i4 = 0; i4 < 28; i4++) { // sliding_loop_start_1_1
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_LOAD_LB(0);
      __builtin_IMCE_STEP();
      short16 var_49 = __builtin_IMCE_GET_CREG((short)0);
      short16 var_50 = __builtin_IMCE_GET_CREG((short)1);
      short16 var_51 = __builtin_IMCE_GET_CREG((short)2);
      short16 var_52 = __builtin_IMCE_GET_CREG((short)3);
      short16 var_53 = __builtin_IMCE_RECV(5);
       var_53 = __builtin_IMCE_ADD(var_49, var_53, 15);
      __builtin_IMCE_SEND(3, var_53, 2, 0);
      short16 var_54 = __builtin_IMCE_RECV(5);
       var_54 = __builtin_IMCE_ADD(var_50, var_54, 15);
      __builtin_IMCE_SEND(3, var_54, 2, 0);
      short16 var_55 = __builtin_IMCE_RECV(5);
       var_55 = __builtin_IMCE_ADD(var_51, var_55, 15);
      __builtin_IMCE_SEND(3, var_55, 2, 0);
      short16 var_56 = __builtin_IMCE_RECV(5);
       var_56 = __builtin_IMCE_ADD(var_52, var_56, 15);
      __builtin_IMCE_SEND(3, var_56, 2, 0);
       var_24 = var_24 + (short)1;
      __builtin_IMCE_SETFLAG(var_24);
    }
  }
}

