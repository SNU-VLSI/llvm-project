let TargetPrefix = "IMCE" in {
  def int_IMCE_SEND : Intrinsic<[], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i32_ty], [ImmArg<ArgIndex<2>>]>;

  // R type
  def int_IMCE_ADD : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i32_ty], [ImmArg<ArgIndex<2>>]>;
  def int_IMCE_SUB : Intrinsic<[llvm_v16i16_ty], [llvm_v16i16_ty, llvm_v16i16_ty, llvm_i32_ty], [ImmArg<ArgIndex<2>>]>;
}