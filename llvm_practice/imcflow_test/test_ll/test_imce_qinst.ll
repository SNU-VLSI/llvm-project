declare void @llvm.IMCE.ADDQ(<16 x i16>, <16 x i16>, i16, i16);
declare void @llvm.IMCE.SUBQ(<16 x i16>, <16 x i16>, i16, i16);
declare void @llvm.IMCE.MULTLQ(<16 x i16>, <16 x i16>, i16, i16);
declare void @llvm.IMCE.MULTHQ(<16 x i16>, <16 x i16>, i16, i16);
declare void @llvm.IMCE.NU.QUANT(<16 x i16>, <16 x i16>, i16, i16);
declare void @llvm.IMCE.MM.QUANT(<16 x i16>, <16 x i16>, i16, i16);
declare <16 x i16> @llvm.IMCE.GET.QREG(i16);

define <16 x i16> @f0(<16 x i16> %a, <16 x i16> %b){
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 0)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 1)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 2)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 3)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 4)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 5)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 6)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 7)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 8)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 9)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 10)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 11)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 12)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 13)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 14)
  call void @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 15)

  %vec0 = call <16 x i16> @llvm.IMCE.GET.QREG(i16 0)
  %vec1 = call <16 x i16> @llvm.IMCE.GET.QREG(i16 1)
  %vec2 = call <16 x i16> @llvm.IMCE.GET.QREG(i16 2)
  %vec3 = call <16 x i16> @llvm.IMCE.GET.QREG(i16 3)

  ret <16 x i16> %vec1
}