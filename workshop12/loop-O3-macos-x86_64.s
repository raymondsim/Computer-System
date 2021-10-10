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
## %bb.2:
	movl	%edx, %r9d
	leaq	-1(%r9), %rax
	movl	%r9d, %r8d
	andl	$3, %r8d
	cmpq	$3, %rax
	jae	LBB0_8
## %bb.3:
	xorl	%edx, %edx
	xorl	%eax, %eax
	testq	%r8, %r8
	jne	LBB0_5
	jmp	LBB0_7
LBB0_1:
	xorl	%eax, %eax
	popq	%rbp
	retq
LBB0_8:
	subq	%r8, %r9
	xorl	%edx, %edx
	xorl	%eax, %eax
	.p2align	4, 0x90
LBB0_9:                                 ## =>This Inner Loop Header: Depth=1
	movslq	(%rsi,%rdx,4), %rcx
	addl	(%rdi,%rcx,4), %eax
	movslq	4(%rsi,%rdx,4), %rcx
	addl	(%rdi,%rcx,4), %eax
	movslq	8(%rsi,%rdx,4), %rcx
	addl	(%rdi,%rcx,4), %eax
	movslq	12(%rsi,%rdx,4), %rcx
	addl	(%rdi,%rcx,4), %eax
	addq	$4, %rdx
	cmpq	%rdx, %r9
	jne	LBB0_9
## %bb.4:
	testq	%r8, %r8
	je	LBB0_7
LBB0_5:
	leaq	(%rsi,%rdx,4), %rcx
	xorl	%edx, %edx
	.p2align	4, 0x90
LBB0_6:                                 ## =>This Inner Loop Header: Depth=1
	movslq	(%rcx,%rdx,4), %rsi
	addl	(%rdi,%rsi,4), %eax
	incq	%rdx
	cmpq	%rdx, %r8
	jne	LBB0_6
LBB0_7:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function

.subsections_via_symbols
