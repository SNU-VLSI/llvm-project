typedef short short16 __attribute__((ext_vector_type(16)));

short16 __builtin_IMCE_ADD(short16, short16, short);

short16 test_vadd(short16 a, short16 b) {
  short16 c = __builtin_IMCE_ADD(a, b, (short)15);
  return c;
}