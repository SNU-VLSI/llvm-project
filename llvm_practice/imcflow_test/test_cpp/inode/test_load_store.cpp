void test_load_store_1(int *a) {
  int data2 = a[0];
  a[1] = data2;
}

void test_load_store_2(int *a) {
  int data2 = a[1 << 20];
  a[(1 << 20) + 1] = data2;
}

void test_load_store_3() {
  int *a = (int *)0x00000007;
  int data2 = a[2];
  a[1] = data2;
}

void test_load_store_4() {
  int *a;
  int data2;
  a = (int *)(32768);
  data2 = a[2];
  a[1] = data2;
}