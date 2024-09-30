declare {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.SUBQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.MULTLQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.MULTHQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.NU_QUANT(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.MM_QUANT(<16 x i16>, <16 x i16>, i16, i16);

define i16 @f0(<16 x i16> %a, <16 x i16> %b){
  %ret1  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 0)
  %ret1.0 = extractvalue {i16, i16, i16, i16} %ret1, 0
  %ret1.1 = extractvalue {i16, i16, i16, i16} %ret1, 1
  %ret1.2 = extractvalue {i16, i16, i16, i16} %ret1, 2
  %ret1.3 = extractvalue {i16, i16, i16, i16} %ret1, 3
  ret i16 %ret1.3 
}