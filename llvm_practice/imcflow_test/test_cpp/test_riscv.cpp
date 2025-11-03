int test_riscv(int *m, int M) {
  int sum = 0;
  for(int i=0; i<M; i++) {
    sum += m[i];
  }
  return sum;
}