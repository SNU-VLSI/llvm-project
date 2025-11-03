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

define i16 @f0(){
entry:
  br label %bb0
bb0:
  ret i16 0
}