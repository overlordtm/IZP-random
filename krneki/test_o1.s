	.file	"test.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d %d %d"
	.text
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
	movl	$0, %eax
	movl	$2, %esi
.L2:
	movl	(%rsp,%rax), %edx
	movl	%edx, %ecx
	shrl	$31, %ecx
	addl	%edx, %ecx
	sarl	%ecx
	addl	$2, %edx
	movl	%edx, (%rsp,%rax)
	leal	2(%rcx), %edx
	movl	%edx, 32(%rsp,%rax)
	movl	%esi, %edx
	subl	%ecx, %edx
	movl	%edx, 16(%rsp,%rax)
	addq	$4, %rax
	cmpq	$12, %rax
	jne	.L2
#APP
# 18 "test.c" 1
	#konec zanka
# 0 "" 2
#NO_APP
	movl	40(%rsp), %r8d
	movl	8(%rsp), %ecx
	movl	24(%rsp), %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk
	movl	$0, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
