	.text
	.file	"Factorial.ll"
	.globl	factorial                       # -- Begin function factorial
	.p2align	4, 0x90
	.type	factorial,@function
factorial:                              # @factorial
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -16
	movl	%edi, 12(%rsp)
	testl	%edi, %edi
	jne	.LBB0_3
# %bb.1:                                # %if.then.1
	movl	$1, %eax
	jmp	.LBB0_2
.LBB0_3:                                # %if.else.1
	movl	12(%rsp), %ebx
	leal	-1(%rbx), %edi
	callq	factorial@PLT
	imull	%ebx, %eax
.LBB0_2:                                # %if.then.1
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	factorial, .Lfunc_end0-factorial
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, 12(%rsp)
	movq	%rsi, 16(%rsp)
	movl	$7, 8(%rsp)
	movl	$7, %edi
	callq	factorial@PLT
	movl	$.L.str.int, %edi
	movl	%eax, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str.int,@object              # @.str.int
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.int:
	.asciz	"%d"
	.size	.L.str.int, 3

	.type	.L.str.double,@object           # @.str.double
.L.str.double:
	.asciz	"%.16e"
	.size	.L.str.double, 6

	.type	.L.str.string,@object           # @.str.string
.L.str.string:
	.asciz	"%s"
	.size	.L.str.string, 3

	.type	.L.str.true,@object             # @.str.true
.L.str.true:
	.asciz	"true"
	.size	.L.str.true, 5

	.type	.L.str.false,@object            # @.str.false
.L.str.false:
	.asciz	"false"
	.size	.L.str.false, 6

	.section	".note.GNU-stack","",@progbits
