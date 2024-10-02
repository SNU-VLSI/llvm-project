declare void @llvm.IMCE.SEND(i16, <16 x i16>, i16, i16)
declare <16 x i16> @llvm.IMCE.RECV(i16)
declare void @llvm.IMCE.SETFLAG(i16)
declare void @llvm.IMCE.STANDBY(i16, i16)

define <16 x i16> @f0_1(<16 x i16> %a){
  call void @llvm.IMCE.SEND(i16 1, <16 x i16> %a, i16 1, i16 1)
  %ret2 = call <16 x i16> @llvm.IMCE.RECV(i16 1)
  call void @llvm.IMCE.SETFLAG(i16 1)
  call void @llvm.IMCE.STANDBY(i16 1, i16 1)
  %ret3 = add <16 x i16> %a, %ret2
  ret <16 x i16> %ret3
}