
	.rdata
str_5:
	.asciiz	"\n"

	.rdata
str_4:
	.asciiz	" v = "

	.rdata
str_3:
	.asciiz	"Posicao "

	.rdata
str_2:
	.asciiz	""

	.rdata
str_1:
	.asciiz	""

	.rdata
str_0:
	.asciiz	""

	.data
	.globl	glob_a
glob_a:
	.word 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	.text
	.globl	func_troca
func_troca:
	subu	$sp, $sp, 132
	sw	$fp, 128($sp)
	add	$fp, $sp, 132
	sw	$ra, -8($fp)
	la	$t0, str_0
	move	$t1, $t0
	li	$v0, 4
	move	$a0, $t1
	syscall
	move	$t0, $t1
	lw	$t1, 0($fp)
	lw	$t0, 4($fp)
	mulo	$t2, $t0, 4
	add	$t0, $t1, $t2
	lw	$t2, 0($t0)
	move	$t0, $t2
	sw	$t0, -12($fp)
	lw	$t0, 0($fp)
	lw	$t2, 8($fp)
	mulo	$t1, $t2, 4
	add	$t2, $t0, $t1
	lw	$t1, 0($t2)
	move	$t2, $t1
	lw	$t1, 0($fp)
	lw	$t0, 4($fp)
	mulo	$t3, $t0, 4
	add	$t0, $t1, $t3
	sw	$t2, 0($t0)
	lw	$t0, -12($fp)
	move	$t2, $t0
	lw	$t0, 0($fp)
	lw	$t3, 8($fp)
	mulo	$t1, $t3, 4
	add	$t3, $t0, $t1
	sw	$t2, 0($t3)
	li	$v0, 0
	lw	$t3, -8($fp)
	move	$ra, $t3
	lw	$t3, -4($fp)
	move	$t2, $t3
	move	$sp, $fp
	move	$fp, $t2
	jr	$ra

	.text
	.globl	func_bubbleSort
func_bubbleSort:
	subu	$sp, $sp, 148
	sw	$fp, 144($sp)
	add	$fp, $sp, 148
	sw	$ra, -8($fp)
	la	$t2, str_1
	move	$t3, $t2
	li	$v0, 4
	move	$a0, $t3
	syscall
	move	$t2, $t3
	li	$t3, 0
	sw	$t3, -12($fp)
label_1:
	li	$t3, 1
	lw	$t2, -12($fp)
	lw	$t1, 4($fp)
	blt	$t2, $t1, label_4
label_25:
	b	label_5
label_5:
	li	$t3, 0
label_4:
	bne	$t3, 0, label_2
label_26:
	b	label_3
label_2:
	li	$t3, 0
	sw	$t3, -16($fp)
label_6:
	li	$t3, 1
	lw	$t1, -16($fp)
	lw	$t2, 4($fp)
	blt	$t1, $t2, label_9
label_27:
	b	label_10
label_10:
	li	$t3, 0
label_9:
	bne	$t3, 0, label_7
label_28:
	b	label_8
label_7:
	li	$t3, 1
	lw	$t2, 0($fp)
	lw	$t1, -12($fp)
	mulo	$t0, $t1, 4
	add	$t1, $t2, $t0
	lw	$t0, 0($t1)
	lw	$t1, 0($fp)
	lw	$t2, -16($fp)
	mulo	$t4, $t2, 4
	add	$t2, $t1, $t4
	lw	$t4, 0($t2)
	blt	$t0, $t4, label_13
label_29:
	b	label_14
label_14:
	li	$t3, 0
label_13:
	bne	$t3, 0, label_11
label_30:
	b	label_12
label_11:
	la	$t3, glob_a
	move	$t4, $t3
	lw	$t3, -12($fp)
	move	$t0, $t3
	lw	$t3, -16($fp)
	move	$t2, $t3
	sw	$t0, -24($fp)
	sw	$t1, -28($fp)
	sw	$t2, -32($fp)
	sw	$t3, -36($fp)
	sw	$t4, -40($fp)
	sw	$t5, -44($fp)
	sw	$t6, -48($fp)
	sw	$t7, -52($fp)
	sw	$t8, -56($fp)
	sw	$t9, -60($fp)
	sw	$v0, -64($fp)
	sw	$v1, -68($fp)
	sw	$a0, -72($fp)
	sw	$a1, -76($fp)
	sw	$a2, -80($fp)
	sw	$a3, -84($fp)
	sw	$s0, -88($fp)
	sw	$s1, -92($fp)
	sw	$s2, -96($fp)
	sw	$s3, -100($fp)
	sw	$s4, -104($fp)
	sw	$s5, -108($fp)
	sw	$s6, -112($fp)
	sw	$s7, -116($fp)
	sw	$t4, 0($sp)
	sw	$t0, 4($sp)
	sw	$t2, 8($sp)
	jal	func_troca
	lw	$t0, -24($fp)
	lw	$t1, -28($fp)
	lw	$t2, -32($fp)
	lw	$t3, -36($fp)
	lw	$t4, -40($fp)
	lw	$t5, -44($fp)
	lw	$t6, -48($fp)
	lw	$t7, -52($fp)
	lw	$t8, -56($fp)
	lw	$t9, -60($fp)
	move	$t2, $v0
	lw	$v0, -64($fp)
	lw	$v1, -68($fp)
	lw	$a0, -72($fp)
	lw	$a1, -76($fp)
	lw	$a2, -80($fp)
	lw	$a3, -84($fp)
	lw	$s0, -88($fp)
	lw	$s1, -92($fp)
	lw	$s2, -96($fp)
	lw	$s3, -100($fp)
	lw	$s4, -104($fp)
	lw	$s5, -108($fp)
	lw	$s6, -112($fp)
	lw	$s7, -116($fp)
	move	$t0, $t2
label_12:
	lw	$t2, -16($fp)
	add	$t0, $t2, 1
	move	$t2, $t0
	sw	$t2, -16($fp)
	b	label_6
label_8:
	lw	$t2, -12($fp)
	add	$t0, $t2, 1
	move	$t2, $t0
	sw	$t2, -12($fp)
	b	label_1
label_3:
	li	$v0, 0
	lw	$t2, -8($fp)
	move	$ra, $t2
	lw	$t2, -4($fp)
	move	$t0, $t2
	move	$sp, $fp
	move	$fp, $t0
	jr	$ra

	.text
	.globl	main
main:
	subu	$sp, $sp, 148
	sw	$fp, 144($sp)
	add	$fp, $sp, 148
	sw	$ra, -8($fp)
	la	$t0, str_2
	move	$t2, $t0
	li	$v0, 4
	move	$a0, $t2
	syscall
	move	$t0, $t2
	li	$t2, 100
	sw	$t2, -20($fp)
	lw	$t2, -20($fp)
	move	$t0, $t2
	sw	$t0, -16($fp)
	li	$t0, 0
	sw	$t0, -12($fp)
label_15:
	li	$t0, 1
	lw	$t2, -16($fp)
	bgt	$t2, 0, label_18
label_31:
	b	label_19
label_19:
	li	$t0, 0
label_18:
	bne	$t0, 0, label_16
label_32:
	b	label_17
label_16:
	lw	$t0, -16($fp)
	move	$t2, $t0
	la	$t0, glob_a
	lw	$t4, -12($fp)
	mulo	$t3, $t4, 4
	add	$t4, $t0, $t3
	sw	$t2, 0($t4)
	lw	$t4, -12($fp)
	add	$t2, $t4, 1
	move	$t4, $t2
	sw	$t4, -12($fp)
	lw	$t4, -16($fp)
	sub	$t2, $t4, 1
	move	$t4, $t2
	sw	$t4, -16($fp)
	b	label_15
label_17:
	la	$t4, glob_a
	move	$t2, $t4
	lw	$t4, -20($fp)
	move	$t3, $t4
	sw	$t0, -28($fp)
	sw	$t1, -32($fp)
	sw	$t2, -36($fp)
	sw	$t3, -40($fp)
	sw	$t4, -44($fp)
	sw	$t5, -48($fp)
	sw	$t6, -52($fp)
	sw	$t7, -56($fp)
	sw	$t8, -60($fp)
	sw	$t9, -64($fp)
	sw	$v0, -68($fp)
	sw	$v1, -72($fp)
	sw	$a0, -76($fp)
	sw	$a1, -80($fp)
	sw	$a2, -84($fp)
	sw	$a3, -88($fp)
	sw	$s0, -92($fp)
	sw	$s1, -96($fp)
	sw	$s2, -100($fp)
	sw	$s3, -104($fp)
	sw	$s4, -108($fp)
	sw	$s5, -112($fp)
	sw	$s6, -116($fp)
	sw	$s7, -120($fp)
	sw	$t2, 0($sp)
	sw	$t3, 4($sp)
	jal	func_bubbleSort
	lw	$t0, -28($fp)
	lw	$t1, -32($fp)
	lw	$t2, -36($fp)
	lw	$t3, -40($fp)
	lw	$t4, -44($fp)
	lw	$t5, -48($fp)
	lw	$t6, -52($fp)
	lw	$t7, -56($fp)
	lw	$t8, -60($fp)
	lw	$t9, -64($fp)
	move	$t3, $v0
	lw	$v0, -68($fp)
	lw	$v1, -72($fp)
	lw	$a0, -76($fp)
	lw	$a1, -80($fp)
	lw	$a2, -84($fp)
	lw	$a3, -88($fp)
	lw	$s0, -92($fp)
	lw	$s1, -96($fp)
	lw	$s2, -100($fp)
	lw	$s3, -104($fp)
	lw	$s4, -108($fp)
	lw	$s5, -112($fp)
	lw	$s6, -116($fp)
	lw	$s7, -120($fp)
	move	$t2, $t3
	li	$t3, 0
	sw	$t3, -16($fp)
label_20:
	li	$t3, 1
	lw	$t2, -16($fp)
	lw	$t4, -20($fp)
	blt	$t2, $t4, label_23
label_33:
	b	label_24
label_24:
	li	$t3, 0
label_23:
	bne	$t3, 0, label_21
label_34:
	b	label_22
label_21:
	la	$t3, str_3
	move	$t4, $t3
	lw	$t3, -16($fp)
	move	$t2, $t3
	la	$t3, str_4
	move	$t0, $t3
	la	$t3, glob_a
	lw	$t1, -16($fp)
	mulo	$t5, $t1, 4
	add	$t1, $t3, $t5
	lw	$t5, 0($t1)
	move	$t1, $t5
	la	$t5, str_5
	move	$t3, $t5
	li	$v0, 4
	move	$a0, $t4
	syscall
	move	$t5, $t4
	li	$v0, 1
	move	$a0, $t2
	syscall
	move	$t4, $t2
	li	$v0, 4
	move	$a0, $t0
	syscall
	move	$t2, $t0
	li	$v0, 1
	move	$a0, $t1
	syscall
	move	$t0, $t1
	li	$v0, 4
	move	$a0, $t3
	syscall
	move	$t1, $t3
	lw	$t3, -16($fp)
	add	$t1, $t3, 1
	move	$t3, $t1
	sw	$t3, -16($fp)
	b	label_20
label_22:
	li	$v0, 0
	lw	$t3, -8($fp)
	move	$ra, $t3
	lw	$t3, -4($fp)
	move	$t1, $t3
	move	$sp, $fp
	move	$fp, $t1
	jr	$ra
