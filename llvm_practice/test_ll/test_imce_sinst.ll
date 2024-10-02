declare <16 x i16> @llvm.IMCE.SCAN.RW(<16 x i16>)

define <16 x i16> @f0_1(<16 x i16> %a){
  %1 = call <16 x i16> @llvm.IMCE.SCAN.RW(<16 x i16> %a)
  %2 = call <16 x i16> @llvm.IMCE.SCAN.RW(<16 x i16> %1)
  ret <16 x i16> %2
}