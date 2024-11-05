void test_load_store_1(int *a) {
  int data2 = a[0];
  a[1] = data2;
}

void test_load_store_2(int *a) {
  int data2 = a[1 << 20];
  a[(1 << 20) + 1] = data2;
}