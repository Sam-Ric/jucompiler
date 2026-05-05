	.text
	.file	"result.ll"
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
