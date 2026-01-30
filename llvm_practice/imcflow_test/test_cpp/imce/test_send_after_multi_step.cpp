#include "common_decl.h"

void test_send_after_multi_step() {  
  short16 var1; 
  short16 var2; 
  short16 var3; 
  short16 var4; 
  short16 var36;
  short16 var37;
  short16 var38;
  short16 var39;
  short16 var40;
  short16 var41;
  short16 var42;
  short16 var43;
  short16 var44;
  short16 var45;
  short16 var46;
  short16 var47;

  short16 var100;
  short16 var101;
  short16 var102;
  short16 var103;
  short16 var104;
  short16 var105;
  short16 var106;
  short16 var107;
  short16 var138;
  short16 var139;
  short16 var140;
  short16 var141;

  short16 var201;
  short16 var202;
  short16 var203;
  short16 var204;
  short16 var238;
  short16 var239;
  short16 var240;
  short16 var241;


  var36 = __builtin_IMCE_RECV(1);
  var37 = __builtin_IMCE_RECV(1);

  __builtin_IMCE_STEP();

  var1 = __builtin_IMCE_GET_CREG((short)0);
  var2 = __builtin_IMCE_GET_CREG((short)1);
  var3 = __builtin_IMCE_GET_CREG((short)2);
  var4 = __builtin_IMCE_GET_CREG((short)3);
  // generate: batch_norm

  var38 = __builtin_IMCE_MULTL(var1, var36, 15);
  var38 = __builtin_IMCE_ADD(var38, var37, 15);
  var39 = __builtin_IMCE_MULTL(var2, var42, 15);
  var39 = __builtin_IMCE_ADD(var39, var43, 15);
  var40 = __builtin_IMCE_MULTL(var3, var44, 15);
  var40 = __builtin_IMCE_ADD(var40, var45, 15);
  var41 = __builtin_IMCE_MULTL(var4, var46, 15);
  var41 = __builtin_IMCE_ADD(var41, var47, 15);

  __builtin_IMCE_STEP();

  var101 = __builtin_IMCE_GET_CREG((short)0);
  var102 = __builtin_IMCE_GET_CREG((short)1);
  var103 = __builtin_IMCE_GET_CREG((short)2);
  var104 = __builtin_IMCE_GET_CREG((short)3);
  // generate: batch_norm

  var138 = __builtin_IMCE_MULTL(var101, var36, 15);
  var138 = __builtin_IMCE_ADD(var138, var37, 15);
  var139 = __builtin_IMCE_MULTL(var102, var42, 15);
  var139 = __builtin_IMCE_ADD(var139, var43, 15);
  var140 = __builtin_IMCE_MULTL(var103, var44, 15);
  var140 = __builtin_IMCE_ADD(var140, var45, 15);
  var141 = __builtin_IMCE_MULTL(var104, var46, 15);
  var141 = __builtin_IMCE_ADD(var141, var47, 15);

  __builtin_IMCE_STEP();

  var201 = __builtin_IMCE_GET_CREG((short)0);
  var202 = __builtin_IMCE_GET_CREG((short)1);
  var203 = __builtin_IMCE_GET_CREG((short)2);
  var204 = __builtin_IMCE_GET_CREG((short)3);
  // generate: batch_norm

  var238 = __builtin_IMCE_MULTL(var201, var36, 15);
  var238 = __builtin_IMCE_ADD(var238, var37, 15);
  var239 = __builtin_IMCE_MULTL(var202, var42, 15);
  var239 = __builtin_IMCE_ADD(var239, var43, 15);
  var240 = __builtin_IMCE_MULTL(var203, var44, 15);
  var240 = __builtin_IMCE_ADD(var240, var45, 15);
  var241 = __builtin_IMCE_MULTL(var204, var46, 15);
  var241 = __builtin_IMCE_ADD(var241, var47, 15);

  // endgenerate: batch_norm
  __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var39, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var40, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var41, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2

  __builtin_IMCE_SEND(1, var138, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var139, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var140, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var141, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2

  __builtin_IMCE_SEND(1, var238, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var239, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var240, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var241, 2, 0); // TensorEdge(((22, 19), odata), (23, data)), imce_3_3 -> imce_3_2

  __builtin_IMCE_STOP();
}