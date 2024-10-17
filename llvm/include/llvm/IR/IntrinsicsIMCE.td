let TargetPrefix = "IMCE" in {
  // R type
  def int_IMCE_ADD   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_SUB   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_AND   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_OR    : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_XOR   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_SRL   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_SLL   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_SRA   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_MAX   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_MIN   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_MULTL : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;
  def int_IMCE_MULTH : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, IntrNoMem]>;

  // I type
  def int_IMCE_ADDI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_SUBI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_ANDI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_ORI    : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_XORI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_SRLI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_SLLI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_SRAI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_MAXI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_MINI   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_MULTLI : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;
  def int_IMCE_MULTHI : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, IntrNoMem]>;

  // A type
  def int_IMCE_DWCONV   : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>, ImmArg<ArgIndex<4>>, IntrNoMem, IntrHasSideEffects]>;

  // B type
  // def int_IMCE_BNE   : Intrinsic<[], [llvm_i16_ty, llvm_v16i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<0>>, ImmArg<ArgIndex<2>>, IntrNoMem]>;

  // C type
  def int_IMCE_SEND     : Intrinsic<[], [llvm_i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<0>>, ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>, IntrHasSideEffects]>;
  def int_IMCE_RECV     : Intrinsic<[llvm_v16i16_ty], [llvm_i16_ty], [ImmArg<ArgIndex<0>>, IntrHasSideEffects]>;
  def int_IMCE_SETFLAG  : Intrinsic<[], [llvm_i16_ty], [ImmArg<ArgIndex<0>>, IntrHasSideEffects]>;
  def int_IMCE_STANDBY  : Intrinsic<[], [llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<0>>, ImmArg<ArgIndex<1>>, IntrHasSideEffects]>;

  // P type
  def int_IMCE_MAXPOOL : Intrinsic<[llvm_v16i16_ty], [llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<0>>, ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>]>;
  def int_IMCE_AVGPOOL : Intrinsic<[llvm_v16i16_ty], [llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<0>>, ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>]>;

  // Q type
  def int_IMCE_ADDQ     : Intrinsic<[llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>]>;
  def int_IMCE_SUBQ     : Intrinsic<[llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>]>;
  def int_IMCE_MULTLQ   : Intrinsic<[llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>]>;
  def int_IMCE_MULTHQ   : Intrinsic<[llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>]>;
  def int_IMCE_NU_QUANT : Intrinsic<[llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>]>;
  def int_IMCE_MM_QUANT : Intrinsic<[llvm_i16_ty, llvm_i16_ty, llvm_i16_ty, llvm_i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i16_ty, llvm_i16_ty], [ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>]>;

  // X type
  def int_IMCE_STEP : Intrinsic<[llvm_v16i16_ty, llvm_v16i16_ty, llvm_v16i16_ty, llvm_v16i16_ty], []>;
  def int_IMCE_NOP  : Intrinsic<[], []>;
  def int_IMCE_STOP : Intrinsic<[], []>;

  // J type
  // def int_IMCE_JUMP : Intrinsic<[], [llvm_i32_ty], [ImmArg<ArgIndex<0>>]>;
  // def int_IMCE_JUMP : Intrinsic<[], [llvm_token_ty], [ImmArg<ArgIndex<0>>]>;
  // def int_IMCE_JUMP : Intrinsic<[], [llvm_anyint_ty], [ImmArg<ArgIndex<0>>]>;


  // S type
  def int_IMCE_SCAN_RW : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty]>;

  def int_IMCE_cloop_begin :
    Intrinsic<[llvm_i32_ty],
              [llvm_i32_ty, llvm_i32_ty],
              [IntrHasSideEffects, IntrNoDuplicate]>;

  def int_IMCE_cloop_end :
    Intrinsic<[llvm_i32_ty, llvm_i32_ty],
              [llvm_i32_ty, llvm_i32_ty],
              [IntrHasSideEffects, IntrNoDuplicate]>;

  // def int_IMCE_cloop_guard :
  //   Intrinsic<[llvm_i32_ty],
  //             [llvm_i32_ty, llvm_i32_ty],
  //             [IntrHasSideEffects, IntrNoDuplicate]>;

}