// Test case for OR to ADD conversion
// This code pattern triggers: (i1*4+1)*32 -> (i1 << 7) | 32
// which previously failed with "Cannot select: or disjoint" error

void test_or_pattern() {
  int *var6 = (int*)(2048);
  int var8 = 0;
  int var7 = var6[0];
  int var9, var10, var11, var12;

  for (int i1 = 0; i1 < var7; i1++) {
    // These address calculations get optimized to shift+or patterns
    var9 = var8 + (i1*4+0)*32;
    var10 = var8 + (i1*4+1)*32;  // This becomes: (i1 << 7) | 32
    var11 = var8 + (i1*4+2)*32;  // This becomes: (i1 << 7) | 64
    var12 = var8 + (i1*4+3)*32;  // This becomes: (i1 << 7) | 96

    asm volatile("" ::: "memory");  // Compiler barrier
    __builtin_INODE_RECV(var9, 0, 0, 2);
    __builtin_INODE_RECV(var10, 0, 0, 2);
    __builtin_INODE_RECV(var11, 0, 0, 2);
    __builtin_INODE_RECV(var12, 0, 0, 2);
  }
}
