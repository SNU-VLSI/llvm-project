declare <16 x i16> @llvm.IMCE.MAXPOOL(i16, i16, i16);
declare <16 x i16> @llvm.IMCE.AVGPOOL(i16, i16, i16);

define <16 x i16> @f0_1(){
  %ret1 = call <16 x i16> @llvm.IMCE.MAXPOOL(i16 1, i16 1, i16 1)
  %ret2 = call <16 x i16> @llvm.IMCE.AVGPOOL(i16 1, i16 1, i16 1)
  %ret3 = add <16 x i16> %ret1, %ret2
  ret <16 x i16> %ret3
}