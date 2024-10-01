int main(void) {
  int x = 0;
  for(int i=0; i<10; i++) {
    for(int j=0; j<5; j++) {
      x += (i+j);
    }
  }
  return x;
}