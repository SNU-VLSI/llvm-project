int test(int *addr) {
  int a = addr[3];
  // bne_update with hardware loop
  for(int i=0; i<10; i++) {
    for(int j=0; j<11; j++) {
      __builtin_INODE_SEND(a, 1, 1, 1);
      a += 6;
    }
  }

  return a;
}