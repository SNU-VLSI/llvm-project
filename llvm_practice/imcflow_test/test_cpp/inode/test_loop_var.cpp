int test(int *addr, int loop_count) {
  int a = addr[3];
  // bne_update with hardware loop
  for(int i=0; i<loop_count; i++) {
    __builtin_INODE_SEND(a, 1, 1, 1);
    a += 6;
  }

  return a;
}