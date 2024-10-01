declare {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.SUBQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.MULTLQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.MULTHQ(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.NU_QUANT(<16 x i16>, <16 x i16>, i16, i16);
declare {i16, i16, i16, i16} @llvm.IMCE.MM_QUANT(<16 x i16>, <16 x i16>, i16, i16);

define <16 x i16> @f0(<16 x i16> %a, <16 x i16> %b){
  %ret1  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 0)
  %ret1.0 = extractvalue {i16, i16, i16, i16} %ret1, 0
  %ret1.1 = extractvalue {i16, i16, i16, i16} %ret1, 1
  %ret1.2 = extractvalue {i16, i16, i16, i16} %ret1, 2
  %ret1.3 = extractvalue {i16, i16, i16, i16} %ret1, 3

  %ret2  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 1)
  %ret2.0 = extractvalue {i16, i16, i16, i16} %ret2, 0
  %ret2.1 = extractvalue {i16, i16, i16, i16} %ret2, 1
  %ret2.2 = extractvalue {i16, i16, i16, i16} %ret2, 2
  %ret2.3 = extractvalue {i16, i16, i16, i16} %ret2, 3

  %ret3  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 2)
  %ret3.0 = extractvalue {i16, i16, i16, i16} %ret3, 0
  %ret3.1 = extractvalue {i16, i16, i16, i16} %ret3, 1
  %ret3.2 = extractvalue {i16, i16, i16, i16} %ret3, 2
  %ret3.3 = extractvalue {i16, i16, i16, i16} %ret3, 3

  %ret4  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 3)
  %ret4.0 = extractvalue {i16, i16, i16, i16} %ret4, 0
  %ret4.1 = extractvalue {i16, i16, i16, i16} %ret4, 1
  %ret4.2 = extractvalue {i16, i16, i16, i16} %ret4, 2
  %ret4.3 = extractvalue {i16, i16, i16, i16} %ret4, 3

  %ret5  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 4)
  %ret5.0 = extractvalue {i16, i16, i16, i16} %ret5, 0
  %ret5.1 = extractvalue {i16, i16, i16, i16} %ret5, 1
  %ret5.2 = extractvalue {i16, i16, i16, i16} %ret5, 2
  %ret5.3 = extractvalue {i16, i16, i16, i16} %ret5, 3

  %ret6  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 5)
  %ret6.0 = extractvalue {i16, i16, i16, i16} %ret6, 0
  %ret6.1 = extractvalue {i16, i16, i16, i16} %ret6, 1
  %ret6.2 = extractvalue {i16, i16, i16, i16} %ret6, 2
  %ret6.3 = extractvalue {i16, i16, i16, i16} %ret6, 3

  %ret7  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 6)
  %ret7.0 = extractvalue {i16, i16, i16, i16} %ret7, 0
  %ret7.1 = extractvalue {i16, i16, i16, i16} %ret7, 1
  %ret7.2 = extractvalue {i16, i16, i16, i16} %ret7, 2
  %ret7.3 = extractvalue {i16, i16, i16, i16} %ret7, 3

  %ret8  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 7)
  %ret8.0 = extractvalue {i16, i16, i16, i16} %ret8, 0
  %ret8.1 = extractvalue {i16, i16, i16, i16} %ret8, 1
  %ret8.2 = extractvalue {i16, i16, i16, i16} %ret8, 2
  %ret8.3 = extractvalue {i16, i16, i16, i16} %ret8, 3
  
  %ret9  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 8)
  %ret9.0 = extractvalue {i16, i16, i16, i16} %ret9, 0
  %ret9.1 = extractvalue {i16, i16, i16, i16} %ret9, 1
  %ret9.2 = extractvalue {i16, i16, i16, i16} %ret9, 2
  %ret9.3 = extractvalue {i16, i16, i16, i16} %ret9, 3

  %ret10  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 9)
  %ret10.0 = extractvalue {i16, i16, i16, i16} %ret10, 0
  %ret10.1 = extractvalue {i16, i16, i16, i16} %ret10, 1
  %ret10.2 = extractvalue {i16, i16, i16, i16} %ret10, 2
  %ret10.3 = extractvalue {i16, i16, i16, i16} %ret10, 3

  %ret11  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 10)
  %ret11.0 = extractvalue {i16, i16, i16, i16} %ret11, 0
  %ret11.1 = extractvalue {i16, i16, i16, i16} %ret11, 1
  %ret11.2 = extractvalue {i16, i16, i16, i16} %ret11, 2
  %ret11.3 = extractvalue {i16, i16, i16, i16} %ret11, 3

  %ret12  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 11)
  %ret12.0 = extractvalue {i16, i16, i16, i16} %ret12, 0
  %ret12.1 = extractvalue {i16, i16, i16, i16} %ret12, 1
  %ret12.2 = extractvalue {i16, i16, i16, i16} %ret12, 2
  %ret12.3 = extractvalue {i16, i16, i16, i16} %ret12, 3

  %ret13  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 12)
  %ret13.0 = extractvalue {i16, i16, i16, i16} %ret13, 0
  %ret13.1 = extractvalue {i16, i16, i16, i16} %ret13, 1
  %ret13.2 = extractvalue {i16, i16, i16, i16} %ret13, 2
  %ret13.3 = extractvalue {i16, i16, i16, i16} %ret13, 3

  %ret14  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 13)
  %ret14.0 = extractvalue {i16, i16, i16, i16} %ret14, 0
  %ret14.1 = extractvalue {i16, i16, i16, i16} %ret14, 1
  %ret14.2 = extractvalue {i16, i16, i16, i16} %ret14, 2
  %ret14.3 = extractvalue {i16, i16, i16, i16} %ret14, 3

  %ret15  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 14)
  %ret15.0 = extractvalue {i16, i16, i16, i16} %ret15, 0
  %ret15.1 = extractvalue {i16, i16, i16, i16} %ret15, 1
  %ret15.2 = extractvalue {i16, i16, i16, i16} %ret15, 2
  %ret15.3 = extractvalue {i16, i16, i16, i16} %ret15, 3

  %ret16  = call {i16, i16, i16, i16} @llvm.IMCE.ADDQ(<16 x i16> %a, <16 x i16> %b, i16 1, i16 15)
  %ret16.0 = extractvalue {i16, i16, i16, i16} %ret16, 0
  %ret16.1 = extractvalue {i16, i16, i16, i16} %ret16, 1
  %ret16.2 = extractvalue {i16, i16, i16, i16} %ret16, 2
  %ret16.3 = extractvalue {i16, i16, i16, i16} %ret16, 3

  %vec0 = insertelement <16 x i16> zeroinitializer, i16 %ret1.0, i32 0
  %vec1 = insertelement <16 x i16> %vec0, i16  %ret2.0, i32   1
  %vec2 = insertelement <16 x i16> %vec1, i16  %ret3.0, i32   2
  %vec3 = insertelement <16 x i16> %vec2, i16  %ret4.0, i32   3
  %vec4 = insertelement <16 x i16> %vec3, i16  %ret5.0, i32   4
  %vec5 = insertelement <16 x i16> %vec4, i16  %ret6.0, i32   5
  %vec6 = insertelement <16 x i16> %vec5, i16  %ret7.0, i32   6
  %vec7 = insertelement <16 x i16> %vec6, i16  %ret8.0, i32   7
  %vec8 = insertelement <16 x i16> %vec7, i16  %ret9.0, i32   8
  %vec9 = insertelement <16 x i16> %vec8, i16  %ret10.0, i32  9
  %vec10 = insertelement <16 x i16> %vec9, i16  %ret11.0, i32 10
  %vec11 = insertelement <16 x i16> %vec10, i16 %ret12.0, i32 11
  %vec12 = insertelement <16 x i16> %vec11, i16 %ret13.0, i32 12
  %vec13 = insertelement <16 x i16> %vec12, i16 %ret14.0, i32 13
  %vec14 = insertelement <16 x i16> %vec13, i16 %ret15.0, i32 14
  %vec15 = insertelement <16 x i16> %vec14, i16 %ret16.0, i32 15

  ret <16 x i16> %vec15 
}