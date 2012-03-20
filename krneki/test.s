	.file	"test.c"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$1, -64(%rbp)
	movl	$2, -60(%rbp)
	movl	$3, -56(%rbp)
	movl	$1, -48(%rbp)
	movl	$2, -44(%rbp)
	movl	$3, -40(%rbp)
	movl	$1, -32(%rbp)
	movl	$2, -28(%rbp)
	movl	$3, -24(%rbp)
	movl	$0x3f99999a, %eax
	movl	%eax, -4(%rbp)
#APP
# 11 "test.c" 1
	#zanka
# 0 "" 2
#NO_APP
	movl	$0, -8(%rbp)
	jmp	.L2
.L3:
	movl	-8(%rbp), %eax
	cltq
	movl	-64(%rbp,%rax,4), %eax
	cvtsi2ss	%eax, %xmm0
	divss	-4(%rbp), %xmm0
	cvttss2si	%xmm0, %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	movl	-8(%rbp), %eax
	cltq
	movl	-64(%rbp,%rax,4), %eax
	cvtsi2ss	%eax, %xmm0
	addss	-4(%rbp), %xmm0
	cvttss2si	%xmm0, %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -64(%rbp,%rax,4)
	movl	-8(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	cvtsi2ss	%eax, %xmm0
	addss	-4(%rbp), %xmm0
	cvttss2si	%xmm0, %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -32(%rbp,%rax,4)
	movl	-8(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	cvtsi2ss	%eax, %xmm0
	movss	-4(%rbp), %xmm1
	movaps	%xmm1, %xmm2
	subss	%xmm0, %xmm2
	movaps	%xmm2, %xmm0
	cvttss2si	%xmm0, %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	addl	$1, -8(%rbp)
.L2:
	cmpl	$2, -8(%rbp)
	jle	.L3
#APP
# 18 "test.c" 1
	#konec zanka
# 0 "" 2
#NO_APP
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
