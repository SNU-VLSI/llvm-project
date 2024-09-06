; ModuleID = 'ch7_ssa.cpp'
source_filename = "ch7_ssa.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@G = dso_local global i32 0, align 4
@H = dso_local global i32 0, align 4

; Function Attrs: mustprogress nounwind uwtable
define dso_local noundef i32 @_Z4testb(i1 noundef zeroext %Condition) #0 {
entry:
  %Condition.addr = alloca i8, align 1
  %X = alloca i32, align 4
  %frombool = zext i1 %Condition to i8
  store i8 %frombool, ptr %Condition.addr, align 1, !tbaa !5
  call void @llvm.lifetime.start.p0(i64 4, ptr %X) #2
  %0 = load i8, ptr %Condition.addr, align 1, !tbaa !5, !range !9, !noundef !10
  %tobool = trunc i8 %0 to i1
  br i1 %tobool, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load i32, ptr @G, align 4, !tbaa !11
  store i32 %1, ptr %X, align 4, !tbaa !11
  br label %if.end

if.else:                                          ; preds = %entry
  %2 = load i32, ptr @H, align 4, !tbaa !11
  store i32 %2, ptr %X, align 4, !tbaa !11
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %3 = load i32, ptr %X, align 4, !tbaa !11
  call void @llvm.lifetime.end.p0(i64 4, ptr %X) #2
  ret i32 %3
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
!11 = !{!12, !12, i64 0}
!12 = !{!"int", !7, i64 0}
