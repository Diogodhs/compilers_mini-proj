; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:w-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-windows-msvc19.22.27905"

@nom_nom = dso_local local_unnamed_addr global i32 88, align 4
@arroz = common dso_local local_unnamed_addr global i32 0, align 4

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @aqua_baldo() local_unnamed_addr #0 {
  ret i32 -159
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @function7(i32, i32) local_unnamed_addr #0 {
  %3 = srem i32 %0, %1
  %4 = add nsw i32 %3, %1
  %5 = mul i32 %4, %0
  %6 = sub i32 0, %5
  %7 = sdiv i32 %6, 2
  %8 = sub nsw i32 %3, %7
  ret i32 %8
}

; Function Attrs: nofree norecurse nounwind uwtable writeonly
define dso_local void @kk(i32) local_unnamed_addr #1 {
  %2 = sdiv i32 %0, 2
  store i32 %2, i32* @arroz, align 4, !tbaa !3
  ret void
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @aster(i32, i32, i32) local_unnamed_addr #0 {
  %4 = mul nsw i32 %0, 3
  %5 = mul nsw i32 %1, 5
  %6 = add nsw i32 %5, %4
  %7 = shl i32 %2, 1
  %8 = add nsw i32 %6, %7
  %9 = sdiv i32 %8, 10
  %10 = mul nsw i32 %0, 255
  %11 = sdiv i32 %10, %9
  %12 = mul nsw i32 %1, 255
  %13 = sdiv i32 %12, %9
  %14 = mul i32 %2, 255
  %15 = add i32 %14, -32385
  %16 = sdiv i32 %15, %9
  %17 = add nsw i32 %13, %11
  %18 = add nsw i32 %17, %16
  ret i32 %18
}

; Function Attrs: nofree norecurse nounwind uwtable writeonly
define dso_local i32 @main() local_unnamed_addr #1 {
  %1 = tail call i32 @aster(i32 123, i32 111, i32 240)
  store i32 -159, i32* @nom_nom, align 4, !tbaa !3
  tail call void @kk(i32 %1)
  ret i32 0
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree norecurse nounwind uwtable writeonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 2}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 9.0.0 (tags/RELEASE_900/final)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
