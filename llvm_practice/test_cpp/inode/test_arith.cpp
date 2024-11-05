int test(int *addr) {
  int a, b, c, d, e;
  a = addr[3];

  // constant
  addr[5] = 1;

  // add inst
  b = a + 1;

  // mul inst
  d = b * 3;

  return d;
}