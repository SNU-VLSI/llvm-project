	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1"
	.file	"add.ll"
	.globl	f1                              # -- Begin function f1
	.p2align	2
	.type	f1,@function
f1:                                     # @f1
	.cfi_startproc
# %bb.0:
	addw	a0, a0, a1
	ret
.Lfunc_end0:
	.size	f1, .Lfunc_end0-f1
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
