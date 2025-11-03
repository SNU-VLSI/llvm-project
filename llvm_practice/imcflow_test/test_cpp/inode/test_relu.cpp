int test() {
  int hid = __builtin_INODE_GET_CORE_HID();

  // set policy table (read 0,1,2,3,4 from sram)
  int policy_table_start_addr = 0;
  int entry_addr = 0;

  __builtin_INODE_PU(policy_table_start_addr, 0, entry_addr, 0);
  __builtin_INODE_PU(policy_table_start_addr, 1, entry_addr, 1);
  __builtin_INODE_PU(policy_table_start_addr, 2, entry_addr, 2);
  __builtin_INODE_PU(policy_table_start_addr, 3, entry_addr, 3);
  __builtin_INODE_PU(policy_table_start_addr, 4, entry_addr, 4);

  // set_flag and halt
  __builtin_INODE_SET_FLAG(1);
  __builtin_INODE_HALT();


  if (hid == 0) { // write to imce
    constexpr int policy_addr = 0;
    constexpr int imm = 0;
    int imem_size = 1024;
      for (int i = 0; i < imem_size; i+=32) {
      __builtin_INODE_WR_IMEM(i, imm, policy_addr); // @[policy_addr] <= data[$rs1 + imm], addr
    };
  }

  if (hid == 0) { // send inode
    constexpr int fifo_id = 1;
    constexpr int policy_addr = 1;
    constexpr int imm = 0;
    int num_send = 4;
    for (int i = 0; i < num_send; i ++) {
      __builtin_INODE_SEND(i, imm, policy_addr, fifo_id); // @[policy_addr] <= data[$rs1 + imm]
    };
  }
  else if (hid == 1) { // recv inode
    constexpr int fifo_id = 1;
    constexpr int imm = 0;
    int num_recv = 32;
    for (int i = 0; i < num_recv; i ++) {
      // TODO: remove policy from RECV
      __builtin_INODE_RECV(i, imm, 0, fifo_id); // data[$rs1 + imm] <= @[fifo_id]
    };
  };

  // set_flag and halt
  __builtin_INODE_SET_FLAG(1);
  __builtin_INODE_HALT();

  return 0;
}