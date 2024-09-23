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

;define <16 x i16> @f0(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.ADD(<16 x i16> %a, <16 x i16> %b, i16 10)
;  %ret2 = add <16 x i16> %ret, %ret
;  ret <16 x i16> %ret2
;}
define i16 @f0_1(i16 %a, i16 %b){
  %ret2 = add i16 %a, %b
  ret i16 %ret2
}
;define <16 x i16> @f1(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.SUB(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f2(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.AND(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f3(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.OR(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f4(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.XOR(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f5(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.SRL(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f6(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.SLL(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f7(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.SRA(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f8(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.MAX(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f9(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.MIN(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f10(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.MULTL(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}
;define <16 x i16> @f11(<16 x i16> %a, <16 x i16> %b){
;  %ret = call <16 x i16> @llvm.IMCE.MULTH(<16 x i16> %a, <16 x i16> %b, i16 10)
;  ret <16 x i16> %ret
;}