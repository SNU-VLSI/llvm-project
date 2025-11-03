declare <16 x i16> @llvm.IMCE.ADD(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SUB(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.AND(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.OR(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.XOR(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRL(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SLL(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.SRA(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MAX(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MIN(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTL(<16 x i16>, <16 x i16>, i16);
declare <16 x i16> @llvm.IMCE.MULTH(<16 x i16>, <16 x i16>, i16);

define <16 x i16> @f0_1(<16 x i16> %a, <16 x i16> %b){
  %ret1 = add <16 x i16> %a, %b
  %ret2 = call <16 x i16> @llvm.IMCE.ADD(<16 x i16>   %ret1, <16 x i16> %ret1, i16 10)
  %ret3 = call <16 x i16> @llvm.IMCE.SUB(<16 x i16>   %ret2, <16 x i16> %ret2, i16 10)
  %ret4 = call <16 x i16> @llvm.IMCE.AND(<16 x i16>   %ret3, <16 x i16> %ret3, i16 10)
  %ret5 = call <16 x i16> @llvm.IMCE.OR(<16 x i16>    %ret4, <16 x i16> %ret4, i16 10)
  %ret6 = call <16 x i16> @llvm.IMCE.XOR(<16 x i16>   %ret5, <16 x i16> %ret5, i16 10)
  %ret7 = call <16 x i16> @llvm.IMCE.SRL(<16 x i16>   %ret6, <16 x i16> %ret6, i16 10)
  %ret8 = call <16 x i16> @llvm.IMCE.SLL(<16 x i16>   %ret7, <16 x i16> %ret7, i16 10)
  %ret9 = call <16 x i16> @llvm.IMCE.SRA(<16 x i16>   %ret8, <16 x i16> %ret8, i16 10)
  %ret10= call <16 x i16> @llvm.IMCE.MAX(<16 x i16>   %ret9, <16 x i16> %ret9, i16 10)
  %ret11= call <16 x i16> @llvm.IMCE.MIN(<16 x i16>   %ret10, <16 x i16> %ret10, i16 10)
  %ret12= call <16 x i16> @llvm.IMCE.MULTL(<16 x i16> %ret11, <16 x i16> %ret11, i16 10)
  %ret13= call <16 x i16> @llvm.IMCE.MULTH(<16 x i16> %ret12, <16 x i16> %ret12, i16 10)
  ret <16 x i16> %ret13
}

define i16 @f0_2(i16 %a, i16 %b){
  %ret1 = add i16 %a, %b
  ret i16 %ret1
}