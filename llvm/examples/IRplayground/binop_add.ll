; ModuleID = 'binop_add.cpp'
source_filename = "binop_add.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress nounwind uwtable
define dso_local void @_Z1mbb(i1 noundef zeroext %r, i1 noundef zeroext %y) #0 {
entry:
  %r.addr = alloca i8, align 1
  %y.addr = alloca i8, align 1
  %l = alloca i8, align 1
  %frombool = zext i1 %r to i8
  store i8 %frombool, ptr %r.addr, align 1, !tbaa !5
  %frombool1 = zext i1 %y to i8
  store i8 %frombool1, ptr %y.addr, align 1, !tbaa !5
  call void @llvm.lifetime.start.p0(i64 1, ptr %l) #2
  %0 = load i8, ptr %y.addr, align 1, !tbaa !5, !range !9, !noundef !10
  %tobool = trunc i8 %0 to i1
  %conv = zext i1 %tobool to i32
  %1 = load i8, ptr %r.addr, align 1, !tbaa !5, !range !9, !noundef !10
  %tobool2 = trunc i8 %1 to i1
  %conv3 = zext i1 %tobool2 to i32
  %add = add nsw i32 %conv, %conv3
  %tobool4 = icmp ne i32 %add, 0
  %frombool5 = zext i1 %tobool4 to i8
  store i8 %frombool5, ptr %l, align 1, !tbaa !5
  call void @llvm.lifetime.end.p0(i64 1, ptr %l) #2
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

attributes #0 = { mustprogress nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 17.0.6 (git@github.com:SNU-VLSI/llvm-project.git cf13f6c4b08298c21e855f3db298c951c28d024e)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"bool", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C++ TBAA"}
!9 = !{i8 0, i8 2}
!10 = !{}
