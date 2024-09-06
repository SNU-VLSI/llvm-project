let TargetPrefix = "IMCE" in {

  // send(v16i16 value, v16i16 cmdaddr, i32 npa)
  def int_IMCE_SEND : Intrinsic<[llvm_void_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i32_ty], [ImmArg<ArgIndex<2>>]>;
}