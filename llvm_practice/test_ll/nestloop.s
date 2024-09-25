	.text
	.syntax unified
	.eabi_attribute	67, "2.09"	@ Tag_conformance
	.eabi_attribute	6, 1	@ Tag_CPU_arch
	.eabi_attribute	8, 1	@ Tag_ARM_ISA_use
	.eabi_attribute	34, 1	@ Tag_CPU_unaligned_access
	.eabi_attribute	17, 1	@ Tag_ABI_PCS_GOT_use
	.eabi_attribute	20, 1	@ Tag_ABI_FP_denormal
	.eabi_attribute	21, 1	@ Tag_ABI_FP_exceptions
	.eabi_attribute	23, 3	@ Tag_ABI_FP_number_model
	.eabi_attribute	24, 1	@ Tag_ABI_align_needed
	.eabi_attribute	25, 1	@ Tag_ABI_align_preserved
	.eabi_attribute	38, 1	@ Tag_ABI_FP_16bit_format
	.eabi_attribute	14, 0	@ Tag_ABI_PCS_R9_use
	.file	"nestloop.ll"
	.globl	main                            @ -- Begin function main
	.p2align	2
	.type	main,%function
	.code	32                              @ @main
main:
	.fnstart
@ %bb.0:                                @ %entry
	sub	sp, sp, #16
	mov	r0, #0
	str	r0, [sp, #8]
	str	r0, [sp, #12]
	str	r0, [sp, #4]
	b	.LBB0_2
.LBB0_1:                                @ %for.end
                                        @   in Loop: Header=BB0_2 Depth=1
	ldr	r1, [sp, #4]
	add	r1, r1, #1
	str	r1, [sp, #4]
.LBB0_2:                                @ %for.cond
                                        @ =>This Loop Header: Depth=1
                                        @     Child Loop BB0_4 Depth 2
	ldr	r1, [sp, #4]
	cmp	r1, #9
	bgt	.LBB0_6
@ %bb.3:                                @ %for.body
                                        @   in Loop: Header=BB0_2 Depth=1
	str	r0, [sp]
.LBB0_4:                                @ %for.cond1
                                        @   Parent Loop BB0_2 Depth=1
                                        @ =>  This Inner Loop Header: Depth=2
	ldr	r1, [sp]
	cmp	r1, #4
	bgt	.LBB0_1
@ %bb.5:                                @ %for.body3
                                        @   in Loop: Header=BB0_4 Depth=2
	ldr	r1, [sp]
	add	r2, r1, #1
	str	r2, [sp]
	ldr	r2, [sp, #4]
	add	r1, r2, r1
	ldr	r2, [sp, #8]
	add	r1, r2, r1
	str	r1, [sp, #8]
	b	.LBB0_4
.LBB0_6:                                @ %for.end7
	ldr	r0, [sp, #8]
	add	sp, sp, #16
	mov	pc, lr
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.fnend
                                        @ -- End function
	.section	".note.GNU-stack","",%progbits
