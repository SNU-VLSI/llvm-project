int test(int *addr) {
  int a = addr[3];

  // add inst
  int b = a + 1;

  // add with big imm
  int c = b + 16384;

  // mul inst
  int d = c * 2;

  // mul with big imm
  int e = d * 16384;

  // big imm
  addr[5] = 16384;

  return e;
}