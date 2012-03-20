	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"%d %d %d"
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
	subq	$64, %rsp
	movl	$1, -64(%rbp)
	movl	$2, -60(%rbp)
	movl	$3, -56(%rbp)
	movl	$1, -48(%rbp)
	movl	$2, -44(%rbp)
	movl	$3, -40(%rbp)
	movl	$1, -32(%rbp)
	movl	$2, -28(%rbp)
	movl	$3, -24(%rbp)
	movl	$0, -8(%rbp)
	movl	$2, -4(%rbp)
#APP
# 12 "test.c" 1
	#zanka
# 0 "" 2
#NO_APP
	movl	$0, -8(%rbp)
	jmp	.L2
.L3:
	movl	-8(%rbp), %eax
	cltq
	movl	-64(%rbp,%rax,4), %eax
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	-4(%rbp)
	movl	%eax, %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	movl	-8(%rbp), %eax
	cltq
	movl	-64(%rbp,%rax,4), %eax
	movl	%eax, %edx
	addl	-4(%rbp), %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -64(%rbp,%rax,4)
	movl	-8(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	movl	%eax, %edx
	addl	-4(%rbp), %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -32(%rbp,%rax,4)
	movl	-8(%rbp), %eax
	cltq
	movl	-48(%rbp,%rax,4), %eax
	movl	-4(%rbp), %edx
	subl	%eax, %edx
	movl	-8(%rbp), %eax
	cltq
	movl	%edx, -48(%rbp,%rax,4)
	addl	$1, -8(%rbp)
.L2:
	cmpl	$2, -8(%rbp)
	jle	.L3
#APP
# 19 "test.c" 1
	#konec zanka
# 0 "" 2
#NO_APP
	movl	-24(%rbp), %ecx
	movl	-56(%rbp), %edx
	movl	-40(%rbp), %esi
	movl	$.LC0, %eax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
