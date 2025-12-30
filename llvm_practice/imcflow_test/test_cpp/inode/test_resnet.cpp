#include "../common_decl.h"
void tvmgen_default_tvmgen_default_imcflow_main_0_round_imcflow_region1_main_0() {
  int hid = __builtin_INODE_GET_CORE_HID();
  int wid = 0;
  int var1; // policy_table_start_address
  int var2; // imem_start_address
  int var3; // imcu_start_address
  int var4; // send_data_base_address
  int* var5; // sm4odata_d5data_send_data_base_address
  int var6; // sm4odata_d5data_tile_loop_count
  int* var7; // s5odata_d6func_out0_recv_data_base_address
  int var8; // s5odata_d6func_out0_tile_loop_count
  int var9; // recv_data_base_address
  if (hid == 0 && wid == 0) { // inode_0_0
    // generate: clear flag before policy update
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag before policy update
    // generate: policy update
    var1 = 0;
    __builtin_INODE_PU(var1, 0, 0, 0);
    __builtin_INODE_PU(var1, 32, 1, 0);
    __builtin_INODE_PU(var1, 64, 2, 0);
    __builtin_INODE_PU(var1, 96, 3, 0);
    __builtin_INODE_PU(var1, 128, 4, 0);
    var1 = 160;
    __builtin_INODE_PU(var1, 0, 0, 1);
    __builtin_INODE_PU(var1, 32, 1, 1);
    __builtin_INODE_PU(var1, 64, 2, 1);
    __builtin_INODE_PU(var1, 96, 3, 1);
    __builtin_INODE_PU(var1, 128, 4, 1);
    var1 = 320;
    __builtin_INODE_PU(var1, 0, 0, 2);
    __builtin_INODE_PU(var1, 32, 1, 2);
    __builtin_INODE_PU(var1, 64, 2, 2);
    __builtin_INODE_PU(var1, 96, 3, 2);
    var1 = 448;
    __builtin_INODE_PU(var1, 0, 0, 3);
    __builtin_INODE_PU(var1, 32, 1, 3);
    __builtin_INODE_PU(var1, 64, 2, 3);
    var1 = 544;
    __builtin_INODE_PU(var1, 0, 0, 4);
    __builtin_INODE_PU(var1, 32, 1, 4);
    // endgenerate: policy update
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    // generate: imem write: imce_0_1
    var2 = 608;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 1);
    // endgenerate
    // endgenerate: imem write: imce_0_1
    // generate: imem write: imce_0_2
    var2 = 640;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 2);
    // endgenerate
    // endgenerate: imem write: imce_0_2
    // generate: imem write: imce_0_3
    var2 = 672;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 3);
    // endgenerate
    // endgenerate: imem write: imce_0_3
    // generate: imem write: imce_0_4
    var2 = 704;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 4);
    // endgenerate
    // endgenerate: imem write: imce_0_4

    __builtin_INODE_SET_FLAG(1);
    // generate: standby for inode_0_0
    __builtin_INODE_STANDBY(5, 1);
    __builtin_INODE_STANDBY(10, 1);
    __builtin_INODE_STANDBY(15, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    // endgenerate: standby for inode_0_0
    // generate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    // generate: send - TensorEdge((-4, odata), (5, data)), inode_0_0 -> imce_3_4
    var5 = (int*)(32768);
    var4 = 0;
    var6 = var5[0];
    for (int i1 = 0; i1 < var6; i1++) { // generate
      __builtin_INODE_SEND(var4 + i1*32, 0, 0, 0);
    } // endgenerate
    // endgenerate: send - TensorEdge((-4, odata), (5, data)), inode_0_0 -> imce_3_4
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
  }
  else if (hid == 1 && wid == 0) { // inode_1_0
    // generate: clear flag before policy update
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag before policy update
    // generate: policy update
    var1 = 0;
    __builtin_INODE_PU(var1, 0, 0, 0);
    __builtin_INODE_PU(var1, 32, 1, 0);
    __builtin_INODE_PU(var1, 64, 2, 0);
    __builtin_INODE_PU(var1, 96, 3, 0);
    var1 = 128;
    __builtin_INODE_PU(var1, 0, 0, 1);
    __builtin_INODE_PU(var1, 32, 1, 1);
    __builtin_INODE_PU(var1, 64, 2, 1);
    __builtin_INODE_PU(var1, 96, 3, 1);
    var1 = 256;
    __builtin_INODE_PU(var1, 0, 0, 2);
    __builtin_INODE_PU(var1, 32, 1, 2);
    __builtin_INODE_PU(var1, 64, 2, 2);
    var1 = 352;
    __builtin_INODE_PU(var1, 0, 0, 3);
    __builtin_INODE_PU(var1, 32, 1, 3);
    var1 = 416;
    __builtin_INODE_PU(var1, 0, 0, 4);
    __builtin_INODE_PU(var1, 32, 1, 4);
    // endgenerate: policy update
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    // generate: imem write: imce_1_1
    var2 = 480;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 0);
    // endgenerate
    // endgenerate: imem write: imce_1_1
    // generate: imem write: imce_1_2
    var2 = 512;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 1);
    // endgenerate
    // endgenerate: imem write: imce_1_2
    // generate: imem write: imce_1_3
    var2 = 544;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 2);
    // endgenerate
    // endgenerate: imem write: imce_1_3
    // generate: imem write: imce_1_4
    var2 = 576;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 3);
    // endgenerate
    // endgenerate: imem write: imce_1_4

    __builtin_INODE_SET_FLAG(1);
    // generate: standby for inode_1_0
    __builtin_INODE_STANDBY(0, 1);
    __builtin_INODE_STANDBY(10, 1);
    __builtin_INODE_STANDBY(15, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    // endgenerate: standby for inode_1_0
    // generate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
  }
  else if (hid == 2 && wid == 0) { // inode_2_0
    // generate: clear flag before policy update
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag before policy update
    // generate: policy update
    var1 = 0;
    __builtin_INODE_PU(var1, 0, 0, 0);
    __builtin_INODE_PU(var1, 32, 1, 0);
    __builtin_INODE_PU(var1, 64, 2, 0);
    __builtin_INODE_PU(var1, 96, 3, 0);
    var1 = 128;
    __builtin_INODE_PU(var1, 0, 0, 1);
    __builtin_INODE_PU(var1, 32, 1, 1);
    __builtin_INODE_PU(var1, 64, 2, 1);
    __builtin_INODE_PU(var1, 96, 3, 1);
    var1 = 256;
    __builtin_INODE_PU(var1, 0, 0, 2);
    __builtin_INODE_PU(var1, 32, 1, 2);
    __builtin_INODE_PU(var1, 64, 2, 2);
    var1 = 352;
    __builtin_INODE_PU(var1, 0, 0, 3);
    __builtin_INODE_PU(var1, 32, 1, 3);
    var1 = 416;
    __builtin_INODE_PU(var1, 0, 0, 4);
    __builtin_INODE_PU(var1, 32, 1, 4);
    // endgenerate: policy update
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    // generate: imem write: imce_2_1
    var2 = 480;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 0);
    // endgenerate
    // endgenerate: imem write: imce_2_1
    // generate: imem write: imce_2_2
    var2 = 512;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 1);
    // endgenerate
    // endgenerate: imem write: imce_2_2
    // generate: imem write: imce_2_3
    var2 = 544;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 2);
    // endgenerate
    // endgenerate: imem write: imce_2_3
    // generate: imem write: imce_2_4
    var2 = 576;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 3);
    // endgenerate
    // endgenerate: imem write: imce_2_4

    __builtin_INODE_SET_FLAG(1);
    // generate: standby for inode_2_0
    __builtin_INODE_STANDBY(0, 1);
    __builtin_INODE_STANDBY(5, 1);
    __builtin_INODE_STANDBY(15, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    // endgenerate: standby for inode_2_0
    // generate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    __builtin_INODE_SET_FLAG(1);
    __builtin_INODE_HALT();
  }
  else if (hid == 3 && wid == 0) { // inode_3_0
    // generate: clear flag before policy update
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag before policy update
    // generate: policy update
    var1 = 8224;
    for (int i1 = 0; i1 < 7; i1++) { // generate
      __builtin_INODE_PU(var1 + i1*32, 0, i1, 0);
    } // endgenerate
    var1 = 8448;
    for (int i1 = 0; i1 < 7; i1++) { // generate
      __builtin_INODE_PU(var1 + i1*32, 0, i1, 1);
    } // endgenerate
    var1 = 8672;
    for (int i1 = 0; i1 < 6; i1++) { // generate
      __builtin_INODE_PU(var1 + i1*32, 0, i1, 2);
    } // endgenerate
    var1 = 8864;
    __builtin_INODE_PU(var1, 0, 0, 3);
    __builtin_INODE_PU(var1, 32, 1, 3);
    __builtin_INODE_PU(var1, 64, 2, 3);
    __builtin_INODE_PU(var1, 96, 3, 3);
    __builtin_INODE_PU(var1, 128, 4, 3);
    var1 = 9024;
    __builtin_INODE_PU(var1, 0, 0, 4);
    __builtin_INODE_PU(var1, 32, 1, 4);
    __builtin_INODE_PU(var1, 64, 2, 4);
    __builtin_INODE_PU(var1, 96, 3, 4);
    __builtin_INODE_PU(var1, 128, 4, 4);
    // endgenerate: policy update
    // generate: standby and intrt
    __builtin_INODE_STANDBY(0, 1);
    __builtin_INODE_STANDBY(5, 1);
    __builtin_INODE_STANDBY(10, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    __builtin_INODE_DONE();
    __builtin_INODE_INTRT(0);
    __builtin_INODE_HALT();
    // endgenerate: standby and intrt
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    // generate: imem write: imce_3_1
    var2 = 9184;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 3);
    // endgenerate
    // endgenerate: imem write: imce_3_1
    // generate: imem write: imce_3_2
    var2 = 9216;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 4);
    // endgenerate
    // endgenerate: imem write: imce_3_2
    // generate: imem write: imce_3_3
    var2 = 9248;
    __builtin_INODE_SET_ADDR_CNT(0);
    // generate. loop count == 1
    __builtin_INODE_WR_IMEM(var2 + 0*32, 0, 5);
    // endgenerate
    // endgenerate: imem write: imce_3_3
    // generate: imem write: imce_3_4
    var2 = 9280;
    __builtin_INODE_SET_ADDR_CNT(0);
    for (int i1 = 0; i1 < 72; i1++) { // generate
      __builtin_INODE_WR_IMEM(var2 + i1*32, 0, 6);
    } // endgenerate
    // endgenerate: imem write: imce_3_4
    // generate: imcu write
    var3 = 0;
    __builtin_INODE_SET_ADDR_CNT(0);
    for (int i1 = 0; i1 < 256; i1++) { // generate
      __builtin_INODE_WR_IMCU(var3 + i1*32, 0, 1);
    } // endgenerate
    // endgenerate: imcu write
    // generate: imce_3_4 compute
    __builtin_INODE_IMCE_COMPUTE(0, 6);
    // endgenerate: imce_3_4 compute
    __builtin_INODE_SET_FLAG(1);
    // generate: standby for inode_3_0
    __builtin_INODE_STANDBY(0, 1);
    __builtin_INODE_STANDBY(5, 1);
    __builtin_INODE_STANDBY(10, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    // endgenerate: standby for inode_3_0
    // generate: clear flag after imce compute enable
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag after imce compute enable
    // generate: send - TensorEdge((-6, config), (5, config)), inode_3_0 -> imce_3_4
    var4 = 8192;
    // generate. loop count == 1
    __builtin_INODE_SEND(var4 + 0*32, 0, 2, 1);
    // endgenerate
    // endgenerate: send - TensorEdge((-6, config), (5, config)), inode_3_0 -> imce_3_4
    // generate: standby and intrt
    __builtin_INODE_STANDBY(0, 1);
    __builtin_INODE_STANDBY(5, 1);
    __builtin_INODE_STANDBY(10, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    __builtin_INODE_DONE();
    __builtin_INODE_INTRT(0);
    __builtin_INODE_HALT();
    // endgenerate: standby and intrt
    // generate: clear flag
    __builtin_INODE_SET_FLAG(0);
    // endgenerate: clear flag
    // generate: recv: TensorID(6, func_out0)
    var7 = (int*)(28672);
    var9 = 0;
    var8 = var7[0];
    for (int i1 = 0; i1 < var8; i1++) { // generate
      __builtin_INODE_RECV(var9 + i1*32, 0, 0, 2);
    } // endgenerate
    // endgenerate: recv: TensorID(6, func_out0)
    // generate: standby and intrt
    __builtin_INODE_STANDBY(0, 1);
    __builtin_INODE_STANDBY(5, 1);
    __builtin_INODE_STANDBY(10, 1);
    __asm__ volatile("nop\n" "nop\n" "nop\n");
    __builtin_INODE_DONE();
    __builtin_INODE_INTRT(0);
    __builtin_INODE_HALT();
    // endgenerate: standby and intrt
  }
}
