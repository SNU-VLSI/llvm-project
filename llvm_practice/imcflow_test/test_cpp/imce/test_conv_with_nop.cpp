#include "common_decl.h"

void test_compute_after_nop() {  short16 var1; // (ConvBlock(gid: 14), 0)
  short16 var2; // (ConvBlock(gid: 14), 1)
  short16 var3; // (ConvBlock(gid: 14), 2)
  short16 var4; // (ConvBlock(gid: 14), 3)
  short16 var33; // (TensorEdge(((18, -19), fused_scale), ((18, 15), fused_scale)), 0)
  short16 var34; // (TensorEdge(((18, -20), fused_bias), ((18, 15), fused_bias)), 0)
  short16 var35; // (BatchNormBlock(gid: 15), 0)
  short16 var36; // (BatchNormBlock(gid: 15), 1)
  short16 var37; // (BatchNormBlock(gid: 15), 2)
  short16 var38; // (BatchNormBlock(gid: 15), 3)
  short16 var39; // (TensorEdge(((18, -19), fused_scale), ((18, 15), fused_scale)), 1)
  short16 var40; // (TensorEdge(((18, -20), fused_bias), ((18, 15), fused_bias)), 1)
  short16 var41; // (TensorEdge(((18, -19), fused_scale), ((18, 15), fused_scale)), 2)
  short16 var42; // (TensorEdge(((18, -20), fused_bias), ((18, 15), fused_bias)), 2)
  short16 var43; // (TensorEdge(((18, -19), fused_scale), ((18, 15), fused_scale)), 3)
  short16 var44; // (TensorEdge(((18, -20), fused_bias), ((18, 15), fused_bias)), 3)

  // generate: TensorEdge(((18, -18), config), ((18, 14), config)), config write

  __builtin_IMCE_RECV_CFG(1);
  // endgenerate: TensorEdge(((18, -18), config), ((18, 14), config)), config write
  // generate: TensorEdge(((18, -19), fused_scale), ((18, 15), fused_scale)), fused_scale write

  var33 = __builtin_IMCE_RECV(1);
  // endgenerate: TensorEdge(((18, -19), fused_scale), ((18, 15), fused_scale)), fused_scale write
  // generate: TensorEdge(((18, -20), fused_bias), ((18, 15), fused_bias)), fused_bias write

  var34 = __builtin_IMCE_RECV(1);
  // endgenerate: TensorEdge(((18, -20), fused_bias), ((18, 15), fused_bias)), fused_bias write
  // generate: conv exec0
  // generate: conv exec0_row_group0_outer_loop(iterate row offset)
  // generate : conv exec0_row_group0_outer_loop(iterate row offset). loop count == 1
  // generate: conv exec0_row_group0_col_group0
  // generate : conv exec0_row_group0_col_group0. loop count == 1

  // generate: load_block
  for (int i1 = 0; i1 < 10; i1++) { // generate : load_block
    for (int i2 = 0; i2 < 4; i2++) { // generate
      __builtin_IMCE_LOAD_LB(0); // TensorEdge((17, odata), ((18, 14), data)), imce_3_4 -> imce_3_3

    } // endgenerate
  } // endgenerate : load_block
  // endgenerate: load_block
  __builtin_IMCE_STEP();


  var1 = __builtin_IMCE_GET_CREG((short)0);
  var2 = __builtin_IMCE_GET_CREG((short)1);
  var3 = __builtin_IMCE_GET_CREG((short)2);
  var4 = __builtin_IMCE_GET_CREG((short)3);
  // generate: batch_norm

  var35 = __builtin_IMCE_MULTL(var1, var33, 15);
  var35 = __builtin_IMCE_ADD(var35, var34, 15);
  var36 = __builtin_IMCE_MULTL(var2, var39, 15);
  var36 = __builtin_IMCE_ADD(var36, var40, 15);
  var37 = __builtin_IMCE_MULTL(var3, var41, 15);
  var37 = __builtin_IMCE_ADD(var37, var42, 15);
  var38 = __builtin_IMCE_MULTL(var4, var43, 15);
  var38 = __builtin_IMCE_ADD(var38, var44, 15);
  // endgenerate: batch_norm
  __builtin_IMCE_STANDBY(17, 1);
  __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  // endgenerate : conv exec0_row_group0_col_group0
  // endgenerate: conv exec0_row_group0_col_group0
  // generate: conv exec0_row_group0_col_group1
  for (int i1 = 0; i1 < 6; i1++) { // generate : conv exec0_row_group0_col_group1

    // generate: load_block
    // generate : load_block. loop count == 1
    for (int i2 = 0; i2 < 4; i2++) { // generate
      __builtin_IMCE_LOAD_LB(0); // TensorEdge((17, odata), ((18, 14), data)), imce_3_4 -> imce_3_3

    } // endgenerate
    // endgenerate : load_block
    // endgenerate: load_block
    __builtin_IMCE_STEP();


    var1 = __builtin_IMCE_GET_CREG((short)0);
    var2 = __builtin_IMCE_GET_CREG((short)1);
    var3 = __builtin_IMCE_GET_CREG((short)2);
    var4 = __builtin_IMCE_GET_CREG((short)3);
    // generate: batch_norm

    var35 = __builtin_IMCE_MULTL(var1, var33, 15);
    var35 = __builtin_IMCE_ADD(var35, var34, 15);
    var36 = __builtin_IMCE_MULTL(var2, var39, 15);
    var36 = __builtin_IMCE_ADD(var36, var40, 15);
    var37 = __builtin_IMCE_MULTL(var3, var41, 15);
    var37 = __builtin_IMCE_ADD(var37, var42, 15);
    var38 = __builtin_IMCE_MULTL(var4, var43, 15);
    var38 = __builtin_IMCE_ADD(var38, var44, 15);
    // endgenerate: batch_norm
    __builtin_IMCE_STANDBY(17, 1);
    __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  } // endgenerate : conv exec0_row_group0_col_group1
  // endgenerate: conv exec0_row_group0_col_group1
  // generate: conv exec0_row_group0_col_group2
  // generate : conv exec0_row_group0_col_group2. loop count == 1

  // generate: load_block
  // loop ignored with loop count == 0 : load_block
  // endgenerate: load_block
  __builtin_IMCE_STEP();


  var1 = __builtin_IMCE_GET_CREG((short)0);
  var2 = __builtin_IMCE_GET_CREG((short)1);
  var3 = __builtin_IMCE_GET_CREG((short)2);
  var4 = __builtin_IMCE_GET_CREG((short)3);
  // generate: batch_norm

  var35 = __builtin_IMCE_MULTL(var1, var33, 15);
  var35 = __builtin_IMCE_ADD(var35, var34, 15);
  var36 = __builtin_IMCE_MULTL(var2, var39, 15);
  var36 = __builtin_IMCE_ADD(var36, var40, 15);
  var37 = __builtin_IMCE_MULTL(var3, var41, 15);
  var37 = __builtin_IMCE_ADD(var37, var42, 15);
  var38 = __builtin_IMCE_MULTL(var4, var43, 15);
  var38 = __builtin_IMCE_ADD(var38, var44, 15);
  // endgenerate: batch_norm
  __builtin_IMCE_STANDBY(17, 1);
  __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  // endgenerate : conv exec0_row_group0_col_group2
  // endgenerate: conv exec0_row_group0_col_group2
  // endgenerate : conv exec0_row_group0_outer_loop(iterate row offset)
  // endgenerate: conv exec0_row_group0_outer_loop(iterate row offset)
  // generate: conv exec0_row_group1_outer_loop(iterate row offset)
  for (int i1 = 0; i1 < 6; i1++) { // generate : conv exec0_row_group1_outer_loop(iterate row offset)
    // generate: conv exec0_row_group1_col_group0
    // generate : conv exec0_row_group1_col_group0. loop count == 1

    // generate: load_block
    for (int i2 = 0; i2 < 2; i2++) { // generate : load_block
      for (int i3 = 0; i3 < 4; i3++) { // generate
        __builtin_IMCE_LOAD_LB(0); // TensorEdge((17, odata), ((18, 14), data)), imce_3_4 -> imce_3_3

      } // endgenerate
    } // endgenerate : load_block
    // endgenerate: load_block
    __builtin_IMCE_STEP();


    var1 = __builtin_IMCE_GET_CREG((short)0);
    var2 = __builtin_IMCE_GET_CREG((short)1);
    var3 = __builtin_IMCE_GET_CREG((short)2);
    var4 = __builtin_IMCE_GET_CREG((short)3);
    // generate: batch_norm

    var35 = __builtin_IMCE_MULTL(var1, var33, 15);
    var35 = __builtin_IMCE_ADD(var35, var34, 15);
    var36 = __builtin_IMCE_MULTL(var2, var39, 15);
    var36 = __builtin_IMCE_ADD(var36, var40, 15);
    var37 = __builtin_IMCE_MULTL(var3, var41, 15);
    var37 = __builtin_IMCE_ADD(var37, var42, 15);
    var38 = __builtin_IMCE_MULTL(var4, var43, 15);
    var38 = __builtin_IMCE_ADD(var38, var44, 15);
    // endgenerate: batch_norm
    __builtin_IMCE_STANDBY(17, 1);
    __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    // endgenerate : conv exec0_row_group1_col_group0
    // endgenerate: conv exec0_row_group1_col_group0
    // generate: conv exec0_row_group1_col_group1
    for (int i2 = 0; i2 < 6; i2++) { // generate : conv exec0_row_group1_col_group1

      // generate: load_block
      // generate : load_block. loop count == 1
      for (int i3 = 0; i3 < 4; i3++) { // generate
        __builtin_IMCE_LOAD_LB(0); // TensorEdge((17, odata), ((18, 14), data)), imce_3_4 -> imce_3_3

      } // endgenerate
      // endgenerate : load_block
      // endgenerate: load_block
      __builtin_IMCE_STEP();


      var1 = __builtin_IMCE_GET_CREG((short)0);
      var2 = __builtin_IMCE_GET_CREG((short)1);
      var3 = __builtin_IMCE_GET_CREG((short)2);
      var4 = __builtin_IMCE_GET_CREG((short)3);
      // generate: batch_norm

      var35 = __builtin_IMCE_MULTL(var1, var33, 15);
      var35 = __builtin_IMCE_ADD(var35, var34, 15);
      var36 = __builtin_IMCE_MULTL(var2, var39, 15);
      var36 = __builtin_IMCE_ADD(var36, var40, 15);
      var37 = __builtin_IMCE_MULTL(var3, var41, 15);
      var37 = __builtin_IMCE_ADD(var37, var42, 15);
      var38 = __builtin_IMCE_MULTL(var4, var43, 15);
      var38 = __builtin_IMCE_ADD(var38, var44, 15);
      // endgenerate: batch_norm
      __builtin_IMCE_STANDBY(17, 1);
      __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
      __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
      __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
      __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    } // endgenerate : conv exec0_row_group1_col_group1
    // endgenerate: conv exec0_row_group1_col_group1
    // generate: conv exec0_row_group1_col_group2
    // generate : conv exec0_row_group1_col_group2. loop count == 1

    // generate: load_block
    // loop ignored with loop count == 0 : load_block
    // endgenerate: load_block
    __builtin_IMCE_STEP();


    var1 = __builtin_IMCE_GET_CREG((short)0);
    var2 = __builtin_IMCE_GET_CREG((short)1);
    var3 = __builtin_IMCE_GET_CREG((short)2);
    var4 = __builtin_IMCE_GET_CREG((short)3);
    // generate: batch_norm

    var35 = __builtin_IMCE_MULTL(var1, var33, 15);
    var35 = __builtin_IMCE_ADD(var35, var34, 15);
    var36 = __builtin_IMCE_MULTL(var2, var39, 15);
    var36 = __builtin_IMCE_ADD(var36, var40, 15);
    var37 = __builtin_IMCE_MULTL(var3, var41, 15);
    var37 = __builtin_IMCE_ADD(var37, var42, 15);
    var38 = __builtin_IMCE_MULTL(var4, var43, 15);
    var38 = __builtin_IMCE_ADD(var38, var44, 15);
    // endgenerate: batch_norm
    __builtin_IMCE_STANDBY(17, 1);
    __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    // endgenerate : conv exec0_row_group1_col_group2
    // endgenerate: conv exec0_row_group1_col_group2
  } // endgenerate : conv exec0_row_group1_outer_loop(iterate row offset)
  // endgenerate: conv exec0_row_group1_outer_loop(iterate row offset)
  // generate: conv exec0_row_group2_outer_loop(iterate row offset)
  // generate : conv exec0_row_group2_outer_loop(iterate row offset). loop count == 1
  // generate: conv exec0_row_group2_col_group0
  for (int i1 = 0; i1 < 8; i1++) { // generate : conv exec0_row_group2_col_group0

    // generate: load_block
    // loop ignored with loop count == 0 : load_block
    // endgenerate: load_block
    __builtin_IMCE_NOP();
    __builtin_IMCE_NOP();

    var1 = __builtin_IMCE_GET_CREG((short)0);
    var2 = __builtin_IMCE_GET_CREG((short)1);
    var3 = __builtin_IMCE_GET_CREG((short)2);
    var4 = __builtin_IMCE_GET_CREG((short)3);
    // generate: batch_norm

    var35 = __builtin_IMCE_MULTL(var1, var33, 15);
    var35 = __builtin_IMCE_ADD(var35, var34, 15);
    var36 = __builtin_IMCE_MULTL(var2, var39, 15);
    var36 = __builtin_IMCE_ADD(var36, var40, 15);
    var37 = __builtin_IMCE_MULTL(var3, var41, 15);
    var37 = __builtin_IMCE_ADD(var37, var42, 15);
    var38 = __builtin_IMCE_MULTL(var4, var43, 15);
    var38 = __builtin_IMCE_ADD(var38, var44, 15);
    // endgenerate: batch_norm
    __builtin_IMCE_SEND(1, var35, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var36, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var37, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
    __builtin_IMCE_SEND(1, var38, 2, 0); // TensorEdge(((18, 15), odata), (19, data)), imce_3_3 -> imce_3_2
  } // endgenerate : conv exec0_row_group2_col_group0
  // endgenerate: conv exec0_row_group2_col_group0
  // endgenerate : conv exec0_row_group2_outer_loop(iterate row offset)
  // endgenerate: conv exec0_row_group2_outer_loop(iterate row offset)
  // endgenerate: conv exec0
  __builtin_IMCE_STOP();
}