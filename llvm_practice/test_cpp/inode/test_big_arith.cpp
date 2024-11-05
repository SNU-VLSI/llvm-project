int test1(int a) {
  int b;
  // add with big imm
  b = a + (1 << 20);
  return b;
}

int test2(int a) {
  int b;
  // add with big imm
  b = a * (1 << 20);
  return b;
}

// int test3(int a) {
//   int b;
//   // add with big imm
//   b = a - (1 << 20);
//   return b;
// }

int test4(int *a) {
  a[0] = (1 << 20);
  return 0;
}