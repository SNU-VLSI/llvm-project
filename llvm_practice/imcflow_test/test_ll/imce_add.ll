; declare <16 x i16> @llvm.IMCE.ADD(<16 x i16>, <16 x i16>, i32);
; declare <16 x i16> @llvm.IMCE.SUB(<16 x i16>, <16 x i16>, i32);
; 
; define <16 x i16> @f1(<16 x i16> %a, <16 x i16> %b) {
;   %ret = call <16 x i16> @llvm.IMCE.ADD(<16 x i16> %a, <16 x i16> %b, i32 15)
;   %ret2 = call <16 x i16> @llvm.IMCE.SUB(<16 x i16> %ret, <16 x i16> %b, i32 13) 
;   ret <16 x i16> %ret2
; }


; define <16 x i16> @f1(<16 x i16> %a, <16 x i16> %b) {
;   %ret = add <16 x i16> %a, %b
;   ret <16 x i16> %ret
; }

define i16 @f1(i16 %a, i16 %b) {
  %ret = add i16 %a, %b
  ret i16 %ret
}