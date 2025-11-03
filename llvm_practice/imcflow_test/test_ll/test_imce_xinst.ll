declare void @llvm.IMCE.STEP()
declare void @llvm.IMCE.NOP()
declare void @llvm.IMCE.STOP()

define <16 x i16> @f0_1(){
  call void @llvm.IMCE.STEP()
  call void @llvm.IMCE.NOP()
  call void @llvm.IMCE.STOP()
  ret <16 x i16> undef
}