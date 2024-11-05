int test(int *addr) {
  int a, val;
  a = addr[3];

  //beq
  val = addr[5];
  if(val == 1) {
    a += 2;
  }

  // //bne
  // val = addr[6];
  // if(val != 1) {
  //   a += 3;
  // }

  // // blt
  // val = addr[7];
  // if(val < 1) {
  //   a += 4;
  // }

  // // bge
  // val = addr[8];
  // if(val >= 1) {
  //   a += 5;
  // }

  return a;
}