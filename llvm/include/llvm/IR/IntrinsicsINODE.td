let TargetPrefix = "INODE" in {
  // C type
  def int_INODE_SEND : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>, IntrHasSideEffects, IntrReadMem]>;
  def int_INODE_RECV : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>, ImmArg<ArgIndex<3>>, IntrHasSideEffects, IntrWriteMem]>;
  def int_INODE_LAYERINIT : Intrinsic<[], [], [IntrHasSideEffects]>;
  def int_INODE_IMCE_COMPUTE : Intrinsic<[], [llvm_i32_ty], [ImmArg<ArgIndex<0>>, IntrHasSideEffects]>;
  def int_INODE_WR_IMEM : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>, IntrHasSideEffects, IntrReadMem]>;
  def int_INODE_WR_IMCU : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>, IntrHasSideEffects, IntrReadMem]>;
  def int_INODE_WR_REG : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<2>>, IntrHasSideEffects, IntrReadMem]>;
  def int_INODE_SET_ADDR_CNT : Intrinsic<[], [llvm_i32_ty], [ImmArg<ArgIndex<0>>, IntrHasSideEffects]>;

  // S type
  def int_INODE_SET_FLAG  : Intrinsic<[], [llvm_i32_ty], [IntrHasSideEffects]>;
  def int_INODE_STANDBY  : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<0>>, IntrHasSideEffects]>;

  // X type
  def int_INODE_DONE : Intrinsic<[], [], [IntrHasSideEffects]>;
  def int_INODE_HALT : Intrinsic<[], [], [IntrHasSideEffects]>;
  def int_INODE_INTRT : Intrinsic<[], [llvm_i32_ty], [ImmArg<ArgIndex<0>>, IntrHasSideEffects]>;

  // U type
  def int_INODE_PU : Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [ImmArg<ArgIndex<1>>, ImmArg<ArgIndex<3>>, IntrHasSideEffects, IntrReadMem]>;

  def int_INODE_cloop_begin :
    Intrinsic<[llvm_i32_ty],
              [llvm_i32_ty],
              [IntrHasSideEffects, IntrNoDuplicate]>;

  def int_INODE_cloop_end :
    Intrinsic<[llvm_i32_ty, llvm_i32_ty],
              [llvm_i32_ty],
              [IntrHasSideEffects, IntrNoDuplicate]>;

  def int_INODE_HID : Intrinsic<[llvm_i32_ty], [], [IntrHasSideEffects]>;
  def int_INODE_WID : Intrinsic<[llvm_i32_ty], [], [IntrHasSideEffects]>;
}