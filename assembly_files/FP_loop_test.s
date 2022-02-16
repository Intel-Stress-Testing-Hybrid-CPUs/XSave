	.file	"FP_loop_test.cpp"
	.intel_syntax noprefix
	.text
	.globl	_Z7run_fpuv
	.type	_Z7run_fpuv, @function
_Z7run_fpuv:
.LFB0:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	pxor	xmm0, xmm0
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z7run_fpuv, .-_Z7run_fpuv
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
