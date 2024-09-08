	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p1"
	.file	"test.c"
	.globl	add                             # -- Begin function add
	.p2align	2
	.type	add,@function
add:                                    # @add
	.cfi_startproc
# %bb.0:                                # %entry
	add	a0, a0, a1
	beqz	a0, .LBB0_2
# %bb.1:                                # %bb1
	li	a0, 1
	ret
.LBB0_2:                                # %bb2
	li	a0, 2
	ret
.Lfunc_end0:
	.size	add, .Lfunc_end0-add
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
