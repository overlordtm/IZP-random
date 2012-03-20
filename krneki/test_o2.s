	.file	"test.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d %d %d"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB13:
	.cfi_startproc
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movl	$1, (%rsp)
	movl	$2, 4(%rsp)
	movl	$3, 8(%rsp)
	movl	$3, 24(%rsp)
	movl	$3, 40(%rsp)
#APP
# 11 "test.c" 1
	#zanka
# 0 "" 2
#NO_APP
	xorl	%eax, %eax
	movl	$1, %ecx
	movl	$2, %esi
.L3:
	movl	%ecx, %edx
	shrl	$31, %edx
	addl	%ecx, %edx
	addl	$2, %ecx
	sarl	%edx
	movl	%ecx, (%rsp,%rax)
	leal	2(%rdx), %ecx
	movl	%ecx, 32(%rsp,%rax)
	movl	%esi, %ecx
	subl	%edx, %ecx
	movl	%ecx, 16(%rsp,%rax)
	addq	$4, %rax
	cmpq	$12, %rax
	je	.L2
	movl	(%rsp,%rax), %ecx
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L2:
#APP
# 18 "test.c" 1
	#konec zanka
# 0 "" 2
#NO_APP
	movl	40(%rsp), %r8d
	movl	8(%rsp), %ecx
	movl	$.LC0, %esi
	movl	24(%rsp), %edx
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	xorl	%eax, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
