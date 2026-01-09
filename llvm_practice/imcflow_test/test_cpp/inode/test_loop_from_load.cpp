void test() {
  int *var6 = (int*)(2048);
  int var8 = 0;
  int var7 = var6[0];
  for (int i1 = 0; i1 < var7; i1++) { // generate
    __builtin_INODE_RECV(var8 + i1*32, 0, 0, 2);
  } // endgenerate
}