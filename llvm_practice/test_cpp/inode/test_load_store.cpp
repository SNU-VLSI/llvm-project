void test_load_store(int *a) {
  // int data1 = a[1];
  // a[2] = data1;
  int data2 = a[1<<20];
  a[(1<<20)+1] = data2;
}