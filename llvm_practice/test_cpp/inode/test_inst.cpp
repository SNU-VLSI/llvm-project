int test(int *addr) {
  int a = addr[3];

  // add inst
  int b = a + 1;

  // add with big imm
  int c = a + 16384;

  // mul inst
  int d = a * 2;

  // mul with big imm
  int e = a * 16384;

  // big imm
  int f = 16384;

  // load and store
  int g = addr[1];
  addr[2] = 1;

  // beq
  int val = addr[5];
  if(val == 1) {
    a += 2;
  }

  // bne
  val = addr[6];
  if(val != 1) {
    a += 3;
  }

  // blt
  val = addr[7];
  if(val < 1) {
    a += 4;
  }

  // bge
  val = addr[8];
  if(val >= 1) {
    a += 5;
  }

  // bne_update with hardware loop
  for(int i=0; i<10; i++) {
    for(int j=0; j<10; j++) {
      // __builtin_INODE_SEND(a, 1, 1, 1);
      a += 6;
    }
  }

  return a;
}