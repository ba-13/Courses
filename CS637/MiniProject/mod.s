	.file	"mod.cpp"
	.text
	.globl	_Z6modexpjjj
	.type	_Z6modexpjjj, @function
_Z6modexpjjj:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movl	%edx, -28(%rbp)
	movl	$1, -4(%rbp)
	movl	$32, -8(%rbp)
.L4:
	cmpl	$0, -8(%rbp)
	jle	.L2
	cmpl	$0, -24(%rbp)
	je	.L2
	movl	-24(%rbp), %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L3
	movl	-4(%rbp), %eax
	imull	-20(%rbp), %eax
	movl	$0, %edx
	divl	-28(%rbp)
	movl	%edx, -4(%rbp)
.L3:
	shrl	-24(%rbp)
	movl	-20(%rbp), %eax
	imull	%eax, %eax
	movl	$0, %edx
	divl	-28(%rbp)
	movl	%edx, -20(%rbp)
	subl	$1, -8(%rbp)
	jmp	.L4
.L2:
	movl	-4(%rbp), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	_Z6modexpjjj, .-_Z6modexpjjj
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$10000007, %edx
	movl	$12, %esi
	movl	$23, %edi
	call	_Z6modexpjjj
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 9.4.0-1ubuntu1~20.04.1) 9.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
