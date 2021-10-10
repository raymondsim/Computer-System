	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12	sdk_version 10, 15, 4
	.globl	__Z7iteratePiS_i        ## -- Begin function _Z7iteratePiS_i
	.p2align	4, 0x90
__Z7iteratePiS_i:                       ## @_Z7iteratePiS_i
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	testl	%edx, %edx
	jle	LBB0_1
## %bb.3:
	movl	%edx, %r8d
	xorl	%edx, %edx
	xorl	%eax, %eax
	.p2align	4, 0x90
LBB0_4:                                 ## =>This Inner Loop Header: Depth=1
	movslq	(%rsi,%rdx,4), %rcx
	addl	(%rdi,%rcx,4), %eax
	incq	%rdx
	cmpq	%rdx, %r8
	jne	LBB0_4
## %bb.2:
	popq	%rbp
	retq
LBB0_1:
	xorl	%eax, %eax
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
