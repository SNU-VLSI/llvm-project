	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p1"
	.file	"test.c"
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	addi	sp, sp, -48
	.cfi_def_cfa_offset 48
	sd	ra, 40(sp)                      # 8-byte Folded Spill
	sd	s0, 32(sp)                      # 8-byte Folded Spill
	.cfi_offset ra, -8
	.cfi_offset s0, -16
	addi	s0, sp, 48
	.cfi_def_cfa s0, 0
	sw	zero, -20(s0)
	sw	a0, -24(s0)
	sd	a1, -32(s0)
	sw	zero, -44(s0)
	ld	a0, -32(s0)
	ld	a0, 8(a0)
	call	atol
	sw	a0, -36(s0)
	lw	a0, -36(s0)
	sw	a0, -40(s0)
	j	.LBB0_1
.LBB0_1:                                # %while.cond
                                        # =>This Inner Loop Header: Depth=1
	lw	a0, -40(s0)
	beqz	a0, .LBB0_6
	j	.LBB0_2
.LBB0_2:                                # %while.body
                                        #   in Loop: Header=BB0_1 Depth=1
	lwu	a0, -40(s0)
	lui	a1, 205
	addiw	a1, a1, -819
	slli	a1, a1, 12
	addi	a1, a1, -819
	mul	a2, a0, a1
	srli	a2, a2, 35
	li	a3, 10
	mulw	a2, a2, a3
	subw	a0, a0, a2
	sw	a0, -48(s0)
	lw	a0, -48(s0)
	lw	a2, -44(s0)
	addw	a0, a0, a2
	sw	a0, -44(s0)
	lwu	a0, -40(s0)
	mul	a0, a0, a1
	srli	a0, a0, 35
	sw	a0, -40(s0)
	lw	a0, -40(s0)
	bnez	a0, .LBB0_5
	j	.LBB0_3
.LBB0_3:                                # %land.lhs.true
                                        #   in Loop: Header=BB0_1 Depth=1
	lw	a0, -44(s0)
	li	a1, 10
	bltu	a0, a1, .LBB0_5
	j	.LBB0_4
.LBB0_4:                                # %if.then
                                        #   in Loop: Header=BB0_1 Depth=1
	lw	a0, -44(s0)
	sw	a0, -40(s0)
	sw	zero, -44(s0)
	j	.LBB0_5
.LBB0_5:                                # %if.end
                                        #   in Loop: Header=BB0_1 Depth=1
	j	.LBB0_1
.LBB0_6:                                # %while.end
	li	a0, 0
	ld	ra, 40(sp)                      # 8-byte Folded Reload
	ld	s0, 32(sp)                      # 8-byte Folded Reload
	addi	sp, sp, 48
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.ident	"clang version 17.0.6 (git@github.com:jang1664/llvm-project.git f204c8aea5610254071eeac7492395861f7c4955)"
	.section	".note.GNU-stack","",@progbits
