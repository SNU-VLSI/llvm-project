	.text
	.file	"imce_add.ll"
	.globl	f1                              | -- Begin function f1
	.align	2
	.type	f1,@function
f1:                                     | @f1
| %bb.0:
	vadd %v2 %v2 %v3
	jmp %s1
.Lfunc_end0:
	.size	f1, .Lfunc_end0-f1
                                        | -- End function
	.section	".note.GNU-stack","",@progbits
