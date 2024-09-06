int jump(int index) {
  void *jump_table[] = {&&label1, &&label2, &&label3};

  // Jump indirectly to the label using the computed address
  goto *jump_table[index];

label1:
  return 0;

label2:
  return 1;

label3:
  return 3;
}