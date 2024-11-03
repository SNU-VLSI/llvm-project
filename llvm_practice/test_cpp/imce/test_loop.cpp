short test() {
  // bne_update with hardware loop
  for(int i=0; i<10; i++) {
    for(int j=0; j<10; j++) {
      __builtin_IMCE_STOP();
    }
  }

  return short(0);
}