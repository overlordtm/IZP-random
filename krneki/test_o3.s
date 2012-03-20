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
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
#APP
# 11 "test.c" 1
	#zanka
# 0 "" 2
#NO_APP
	movl	$5, 8(%rsp)
	movl	$3, 24(%rsp)
#APP
# 18 "test.c" 1
	#konec zanka
# 0 "" 2
#NO_APP
	movl	$3, %r8d
	movl	$5, %ecx
	movl	$1, %edx
	movl	$.LC0, %esi
	movl	$1, %edi
	xorl	%eax, %eax
	call	__printf_chk
	xorl	%eax, %eax
	addq	$40, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE13:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
