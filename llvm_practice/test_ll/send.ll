declare void @llvm.IMCE.SEND(<16 x i16>, <16 x i16>, i32);

define i32 @f1(<16 x i16> %a, <16 x i16> %b) {
  call void @llvm.IMCE.SEND(<16 x i16> %a, <16 x i16> %b, i32 1)
  ret i32 0
}