; ModuleID = 'indirectbr.cpp'
source_filename = "indirectbr.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@__const._Z4jumpi.jump_table = private unnamed_addr constant [3 x ptr] [ptr blockaddress(@_Z4jumpi, %label1), ptr blockaddress(@_Z4jumpi, %label2), ptr blockaddress(@_Z4jumpi, %label3)], align 16

; Function Attrs: mustprogress nounwind uwtable
define dso_local noundef i32 @_Z4jumpi(i32 noundef %index) #0 {
entry:
  %retval = alloca i32, align 4
  %index.addr = alloca i32, align 4
  %jump_table = alloca [3 x ptr], align 16
  store i32 %index, ptr %index.addr, align 4, !tbaa !5
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %jump_table, ptr align 16 @__const._Z4jumpi.jump_table, i64 24, i1 false)
  %0 = load i32, ptr %index.addr, align 4, !tbaa !5
  %idxprom = sext i32 %0 to i64
  %arrayidx = getelementptr inbounds [3 x ptr], ptr %jump_table, i64 0, i64 %idxprom
  %1 = load ptr, ptr %arrayidx, align 8, !tbaa !9
  br label %indirectgoto

label1:                                           ; preds = %indirectgoto
  store i32 0, ptr %retval, align 4
  br label %return

label2:                                           ; preds = %indirectgoto
  store i32 1, ptr %retval, align 4
  br label %return

label3:                                           ; preds = %indirectgoto
  store i32 3, ptr %retval, align 4
  br label %return

return:                                           ; preds = %label3, %label2, %label1
  %2 = load i32, ptr %retval, align 4
  ret i32 %2

indirectgoto:                                     ; preds = %entry
  %indirect.goto.dest = phi ptr [ %1, %entry ]
  indirectbr ptr %indirect.goto.dest, [label %label1, label %label2, label %label3]
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { mustprogress nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 17.0.6 (git@github.com:SNU-VLSI/llvm-project.git cf13f6c4b08298c21e855f3db298c951c28d024e)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C++ TBAA"}
!9 = !{!10, !10, i64 0}
!10 = !{!"any pointer", !7, i64 0}
