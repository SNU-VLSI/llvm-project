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
  %i2 = phi i16 [0, %entry], [%i_post2, %bb2]
  br label %bb1
bb1:
  %i = phi i16 [0, %bb0], [%i_post, %bb1]
  %i_post = add i16 %i, 1
  %cond = icmp ne i16 %i_post, 10 
  br i1 %cond, label %bb1, label %bb2
bb2:
  %i_post2 = add i16 %i2, 1
  %cond2 = icmp ne i16 %i_post2, 3 
  br i1 %cond2, label %bb0, label %bb3
bb3:
  ret i16 0
}