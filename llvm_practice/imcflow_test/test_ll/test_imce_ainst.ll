declare <16 x i16> @llvm.IMCE.DWCONV(<16 x i16>, i16, i16, i16, i16);

define <16 x i16> @f0(<16 x i16> %a, <16 x i16> %b){
  call <16 x i16> @llvm.IMCE.DWCONV(<16 x i16> %a, i16 0, i16 0, i16 0, i16 0)
  %ret0 = call <16 x i16> @llvm.IMCE.DWCONV(<16 x i16> %b, i16 1, i16 1, i16 1, i16 1)
  ret <16 x i16> %ret0
}