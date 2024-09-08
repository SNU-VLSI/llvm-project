; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p:64:64-i64:64-i128:128-n32:64-S128"
target triple = "riscv64"

; Function Attrs: noinline nounwind optnone uwtable
define i32 @add(i32 %a, i32 %b) {
entry:
  %c = add i32 %a, %b
  %cmp = icmp ne i32 %c, 0
  br i1 %cmp, label %bb1, label %bb2

bb1:
  ret i32 1

bb2:
  ret i32 2
}