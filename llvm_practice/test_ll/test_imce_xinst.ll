declare {<16 x i16>, <16 x i16>, <16 x i16>, <16 x i16>} @llvm.IMCE.STEP()
declare void @llvm.IMCE.NOP()
declare void @llvm.IMCE.STOP()

define <16 x i16> @f0_1(){
  %ret1 = call {<16 x i16>, <16 x i16>, <16 x i16>, <16 x i16>} @llvm.IMCE.STEP()
  %ret1.0 = extractvalue {<16 x i16>, <16 x i16>, <16 x i16>, <16 x i16>} %ret1, 0
  %ret1.1 = extractvalue {<16 x i16>, <16 x i16>, <16 x i16>, <16 x i16>} %ret1, 1
  %ret1.2 = extractvalue {<16 x i16>, <16 x i16>, <16 x i16>, <16 x i16>} %ret1, 2
  %ret1.3 = extractvalue {<16 x i16>, <16 x i16>, <16 x i16>, <16 x i16>} %ret1, 3
  %ret2 = add <16 x i16> %ret1.0, %ret1.1
  %ret3 = add <16 x i16> %ret2, %ret1.2
  %ret4 = add <16 x i16> %ret3, %ret1.3
  call void @llvm.IMCE.NOP()
  call void @llvm.IMCE.STOP()
  ret <16 x i16> %ret4
}