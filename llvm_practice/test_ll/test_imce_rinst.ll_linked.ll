; ModuleID = 'llvm-link'
source_filename = "llvm-link"

define <16 x i16> @f1(<16 x i16> %a, <16 x i16> %b) {
  %ret = call <16 x i16> @llvm.IMCE.ADD(<16 x i16> %a, <16 x i16> %b, i32 10)
  ret <16 x i16> %ret
}

declare <16 x i16> @llvm.IMCE.ADD(<16 x i16>, <16 x i16>, i32)
