define i16 @f1(i16 %a, i16 %b) {
  %ret = add i16 %a, %b
  %ret2 = add i16 %ret, 3
  ret i16 %ret2
}