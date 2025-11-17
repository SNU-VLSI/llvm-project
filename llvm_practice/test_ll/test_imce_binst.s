	.text
	.file	"test_imce_binst.ll"
	.globl	f0                              | -- Begin function f0
	.align	2
	.type	f0,@function
f0:                                     | @f0
| %bb.0:                                | %entry
.LBB0_1:                                | %bb0
                                        | =>This Loop Header: Depth=1
                                        |     Child Loop BB0_2 Depth 2
.LBB0_2:                                | %bb1
                                        |   Parent Loop BB0_1 Depth=1
                                        | =>  This Inner Loop Header: Depth=2
	bne .LBB0_2 %hwlr1 10
	jmp .LBB0_3
.LBB0_3:                                | %bb2
                                        |   in Loop: Header=BB0_1 Depth=1
	bne .LBB0_1 %hwlr0 3
	jmp .LBB0_4
.LBB0_4:                                | %bb3
	vaddi %v2 %v0 0
	jmp %v1
.Lfunc_end0:
	.size	f0, .Lfunc_end0-f0
                                        | -- End function
	.section	".note.GNU-stack","",@progbits
