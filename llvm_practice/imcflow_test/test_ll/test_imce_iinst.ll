declare <16 x i16> @llvm.IMCE.ADDI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SUBI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.ANDI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.ORI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.XORI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRLI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SLLI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRAI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MAXI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MINI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTLI(<16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTHI(<16 x i16>, i16);

define <16 x i16> @f0(<16 x i16> %a){
  %ret1  = call <16 x i16> @llvm.IMCE.ADDI(<16 x i16> %a, i16 1)
  %ret2  = call <16 x i16> @llvm.IMCE.ADDI(<16 x i16> %ret1, i16 2)
  %ret3  = call <16 x i16> @llvm.IMCE.SUBI(<16 x i16> %ret2, i16 3)
  %ret4  = call <16 x i16> @llvm.IMCE.ANDI(<16 x i16> %ret3, i16 4)
  %ret5  = call <16 x i16> @llvm.IMCE.ORI(<16 x i16> %ret4, i16 5)
  %ret6  = call <16 x i16> @llvm.IMCE.XORI(<16 x i16> %ret5, i16 6)
  %ret7  = call <16 x i16> @llvm.IMCE.SRLI(<16 x i16> %ret6, i16 7)
  %ret8  = call <16 x i16> @llvm.IMCE.SLLI(<16 x i16> %ret7, i16 8)
  %ret9  = call <16 x i16> @llvm.IMCE.SRAI(<16 x i16> %ret8, i16 9)
  %ret10 = call <16 x i16> @llvm.IMCE.MAXI(<16 x i16> %ret8, i16 10)
  %ret11 = call <16 x i16> @llvm.IMCE.MINI(<16 x i16> %ret10, i16 11)
  %ret12 = call <16 x i16> @llvm.IMCE.MULTLI(<16 x i16> %ret11, i16 12)
  %ret13 = call <16 x i16> @llvm.IMCE.MULTHI(<16 x i16> %ret12, i16 13)
  ret <16 x i16> %ret12
}