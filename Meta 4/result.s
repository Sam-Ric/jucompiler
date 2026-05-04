	.text
	.file	"result.ll"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3, 0x0                          # -- Begin function main
.LCPI0_0:
	.quad	0x3e4a69ff1b555051              # double 1.2299999999999999E-8
.LCPI0_1:
	.quad	0x4024000000000000              # double 10
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%r15
	pushq	%r14
	pushq	%r12
	pushq	%rbx
	subq	$48, %rsp
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	movl	%edi, -40(%rbp)
	movq	%rsi, -48(%rbp)
	movq	a@GOTPCREL(%rip), %r15
	movl	$2, (%r15)
	movl	$.L.str.int, %edi
	movl	$2, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$3, (%r15)
	movq	d@GOTPCREL(%rip), %r14
	movabsq	$4613937818241073152, %rax      # imm = 0x4008000000000000
	movq	%rax, (%r14)
	movl	$.L.str.int, %edi
	movl	$3, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movsd	(%r14), %xmm0                   # xmm0 = mem[0],zero
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.1, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	(%r15), %eax
	incl	%eax
	movl	%eax, (%r15)
	testl	%eax, %eax
	setg	-33(%rbp)
	jg	.LBB0_2
# %bb.1:                                # %or.right.1
	movl	(%r15), %eax
	addl	$3, %eax
	movl	%eax, (%r15)
	testl	%eax, %eax
	setg	-33(%rbp)
.LBB0_2:                                # %or.end.1
	movl	$.L.str.true, %r12d
	movl	$.L.str.false, %ebx
	cmpb	$0, -33(%rbp)
	movl	$.L.str.false, %esi
	cmovneq	%r12, %rsi
	movl	$.L.str.string, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	(%r15), %esi
	movl	$.L.str.int, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rax
	movq	%rax, %rsp
	incl	(%r15)
	sets	-16(%rcx)
	jns	.LBB0_4
# %bb.3:                                # %and.right.2
	addl	$3, (%r15)
	sets	(%rax)
.LBB0_4:                                # %and.end.2
	cmpb	$0, (%rax)
	cmovneq	%r12, %rbx
	movl	$.L.str.string, %edi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	(%r15), %esi
	movl	$.L.str.int, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movabsq	$4488516523024273489, %rax      # imm = 0x3E4A69FF1B555051
	movq	%rax, (%r14)
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = [1.2299999999999999E-8,0.0E+0]
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movq	$0, (%r14)
	movl	$.L.str.double, %edi
	xorps	%xmm0, %xmm0
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	movabsq	$4621819117588971520, %rax      # imm = 0x4024000000000000
	movq	%rax, (%r14)
	movsd	.LCPI0_1(%rip), %xmm0           # xmm0 = [1.0E+1,0.0E+0]
	movl	$.L.str.double, %edi
	movb	$1, %al
	callq	printf@PLT
	movl	$.L.str.string, %edi
	movl	$.L.strlit.0, %esi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	leaq	-32(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
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

	.type	.L.strlit.0,@object             # @.strlit.0
.L.strlit.0:
	.asciz	"\n"
	.size	.L.strlit.0, 2

	.type	.L.strlit.1,@object             # @.strlit.1
	.section	.rodata.str1.16,"aMS",@progbits,1
	.p2align	4, 0x0
.L.strlit.1:
	.asciz	"Teste\tde \fstring\\\"\r%%"
	.size	.L.strlit.1, 22

	.type	a,@object                       # @a
	.bss
	.globl	a
	.p2align	2, 0x0
a:
	.long	0                               # 0x0
	.size	a, 4

	.type	b,@object                       # @b
	.globl	b
	.p2align	2, 0x0
b:
	.long	0                               # 0x0
	.size	b, 4

	.type	c,@object                       # @c
	.globl	c
	.p2align	2, 0x0
c:
	.long	0                               # 0x0
	.size	c, 4

	.type	d,@object                       # @d
	.globl	d
	.p2align	2, 0x0
d:
	.long	0                               # 0x0
	.size	d, 4

	.section	".note.GNU-stack","",@progbits
