typedef short short16 __attribute__((ext_vector_type(16)));

typedef struct QTypeResult_ {
  short a;
  short b;
  short c;
  short d;
} QTypeResult;

typedef struct StepResult_ {
  short16 a;
  short16 b;
  short16 c;
  short16 d;
} StepResult;

// R inst
short16 __builtin_IMCE_ADD(short16, short16, short);
short16 __builtin_IMCE_SUB(short16, short16, short);
short16 __builtin_IMCE_AND(short16, short16, short);
short16 __builtin_IMCE_OR(short16, short16, short);
short16 __builtin_IMCE_XOR(short16, short16, short);
short16 __builtin_IMCE_SRL(short16, short16, short);
short16 __builtin_IMCE_SLL(short16, short16, short);
short16 __builtin_IMCE_SRA(short16, short16, short);
short16 __builtin_IMCE_MAX(short16, short16, short);
short16 __builtin_IMCE_MIN(short16, short16, short);
short16 __builtin_IMCE_MULTL(short16, short16, short);
short16 __builtin_IMCE_MULTH(short16, short16, short);

// I inst
short16 __builtin_IMCE_ADDI(short16, short);
short16 __builtin_IMCE_SUBI(short16, short);
short16 __builtin_IMCE_ANDI(short16, short);
short16 __builtin_IMCE_ORI(short16, short);
short16 __builtin_IMCE_XORI(short16, short);
short16 __builtin_IMCE_SRLI(short16, short);
short16 __builtin_IMCE_SLLI(short16, short);
short16 __builtin_IMCE_SRAI(short16, short);
short16 __builtin_IMCE_MAXI(short16, short);
short16 __builtin_IMCE_MINI(short16, short);
short16 __builtin_IMCE_MULTLI(short16, short);
short16 __builtin_IMCE_MULTHI(short16, short);

// A type
short16 __builtin_IMCE_DWCONV(short16, short, short, short, short);

// C type
void __builtin_IMCE_SEND(short, short16, short, short);
short16 __builtin_IMCE_RECV(short);
void __builtin_IMCE_SETFLAG(short);
void __builtin_IMCE_STANDBY(short, short);

short16 __builtin_IMCE_MAXPOOL(short, short, short);
short16 __builtin_IMCE_AVGPOOL(short, short, short);

QTypeResult __builtin_IMCE_ADDQ(short16, short16, short, short);
QTypeResult __builtin_IMCE_SUBQ(short16, short16, short, short);
QTypeResult __builtin_IMCE_MULTLQ(short16, short16, short, short);
QTypeResult __builtin_IMCE_MULTHQ(short16, short16, short, short);
QTypeResult __builtin_IMCE_NU_QUANT(short16, short16, short, short);
QTypeResult __builtin_IMCE_MM_QUANT(short16, short16, short, short);

StepResult __builtin_IMCE_STEP(void);
void __builtin_IMCE_NOP(void);
void __builtin_IMCE_STOP(void);

short16 __builtin_IMCE_SCAN_RW(short16);

void __builtin_IMCE_LOAD_LB(short);