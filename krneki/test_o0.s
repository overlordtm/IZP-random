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
	# X[] = {1,2,3}
	movl	$1, -64(%rbp)
	movl	$2, -60(%rbp)
	movl	$3, -56(%rbp)
	# Y[] = {1,2,3}
	movl	$1, -48(%rbp)
	movl	$2, -44(%rbp)
	movl	$3, -40(%rbp)
	# Z[] = {1,2,3}
	movl	$1, -32(%rbp)
	movl	$2, -28(%rbp)
	movl	$3, -24(%rbp)
	# c = 2
	movl	$2, -4(%rbp)
#APP
# 11 "test.c" 1
	#zanka
# 0 "" 2
#NO_APP
# for(i = 0 ...)
	movl	$0, -8(%rbp) # i=0 
	jmp	.L2
.L3:
	movl	-8(%rbp), %eax ; move i into eax 
	cltq	; sign extend eax
	movl	-64(%rbp,%rax,4), %eax ; eax <- X[i]
	movl	%eax, %edx ; edx <- eax
	sarl	$31, %edx ; edx >> 31 (clear edx?)
	idivl	-4(%rbp) ; divide eax with c
	movl	%eax, %edx ; edx <- eax
	movl	-8(%rbp), %eax ; eax <- i
	cltq
	movl	%edx, -48(%rbp,%rax,4) ; Y[i] <- edx
	movl	-8(%rbp), %eax ; move i into eax
	cltq
	movl	-64(%rbp,%rax,4), %eax ; eax <- X[i]
	movl	%eax, %edx ; edx <- eax
	addl	-4(%rbp), %edx ; edx <- edx + c
	movl	-8(%rbp), %eax ; eax <- i
	cltq
	movl	%edx, -64(%rbp,%rax,4) ; X[i] <- edx
	movl	-8(%rbp), %eax ; eax <- i
	cltq
	movl	-48(%rbp,%rax,4), %eax ; eax <- Y[i]
	movl	%eax, %edx ; edx <- eax
	addl	-4(%rbp), %edx ; edx <- edx + c
	movl	-8(%rbp), %eax ; eax <- i
	cltq
	movl	%edx, -32(%rbp,%rax,4) ; X[i] <- edx
	movl	-8(%rbp), %eax ; eax <- i
	cltq
	movl	-48(%rbp,%rax,4), %eax ; eax <- Y[i]
	movl	-4(%rbp), %edx ; edx <- c
	subl	%eax, %edx ; edx <- edx - eax (Y[i])
	movl	-8(%rbp), %eax; eax <- i
	cltq
	movl	%edx, -48(%rbp,%rax,4) ; Y[i] <- edx
	addl	$1, -8(%rbp) ; i++
.L2:
	cmpl	$2, -8(%rbp) # i < 3?
	jle	.L3 # jump less eq
#APP
# 18 "test.c" 1
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
