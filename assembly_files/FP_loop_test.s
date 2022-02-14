	.file	"FP_loop_test.cpp"
	.text
	.globl	init
	.data
	.align 4
	.type	init, @object
	.size	init, 4
init:
	.long	1065353216
	.text
	.globl	_Z7run_fpuv
	.type	_Z7run_fpuv, @function
_Z7run_fpuv:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movss	init(%rip), %xmm1
	movss	.LC0(%rip), %xmm0
	mulss	%xmm1, %xmm0
	movss	init(%rip), %xmm1
	addss	%xmm1, %xmm0
	movss	%xmm0, init(%rip)
	movss	init(%rip), %xmm0
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z7run_fpuv, .-_Z7run_fpuv
	.section	.rodata
	.align 4
.LC0:
	.long	1046777482
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
