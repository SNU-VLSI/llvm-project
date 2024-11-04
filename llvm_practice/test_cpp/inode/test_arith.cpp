int test(int *addr) {
  int a = addr[3];

  // add inst
  // int b = a + 1;

  // add with big imm
  // int c = b + (1<<20)+1;
  int c = a + -((1<<20)+1);

  // mul inst
  // int d = a * 3;

  // mul with big imm
  // int e = a * ((1<<20)+1);
  // int e = a * -((1<<20)+1);

  // big imm
  // addr[5] = (0b11<<21) + 1234;

  return c;
}