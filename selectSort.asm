
	.rdata
str_4:
	.asciiz	"\n"

	.rdata
str_3:
	.asciiz	" v = "

	.rdata
str_2:
	.asciiz	"Posicao "

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
	.globl	func_selectSort
func_selectSort:
	subu	$sp, $sp, 144
	sw	$fp, 140($sp)
	add	$fp, $sp, 144
	sw	$ra, -8($fp)
	la	$t0, str_0
	move	$t1, $t0
	li	$v0, 4
	move	$a0, $t1
	syscall
	move	$t0, $t1
	li	$t1, 0
	sw	$t1, -12($fp)
label_1:
	li	$t1, 1
	lw	$t0, -12($fp)
	lw	$t2, 4($fp)
	sub	$t3, $t2, 1
	blt	$t0, $t3, label_4
label_29:
	b	label_5
label_5:
	li	$t1, 0
label_4:
	bne	$t1, 0, label_2
label_30:
	b	label_3
label_2:
	lw	$t1, -12($fp)
	move	$t3, $t1
	sw	$t3, -20($fp)
	lw	$t3, -12($fp)
	add	$t1, $t3, 1
	move	$t3, $t1
	sw	$t3, -16($fp)
label_6:
	li	$t3, 1
	lw	$t1, -16($fp)
	lw	$t0, 4($fp)
	blt	$t1, $t0, label_9
label_31:
	b	label_10
label_10:
	li	$t3, 0
label_9:
	bne	$t3, 0, label_7
label_32:
	b	label_8
label_7:
	li	$t3, 1
	lw	$t0, 0($fp)
	lw	$t1, -16($fp)
	mulo	$t2, $t1, 4
	add	$t1, $t0, $t2
	lw	$t2, 0($t1)
	lw	$t1, 0($fp)
	lw	$t0, -20($fp)
	mulo	$t4, $t0, 4
	add	$t0, $t1, $t4
	lw	$t4, 0($t0)
	blt	$t2, $t4, label_13
label_33:
	b	label_14
label_14:
	li	$t3, 0
label_13:
	bne	$t3, 0, label_11
label_34:
	b	label_12
label_11:
	lw	$t3, -16($fp)
	move	$t4, $t3
	sw	$t4, -20($fp)
label_12:
	lw	$t4, -16($fp)
	add	$t3, $t4, 1
	move	$t4, $t3
	sw	$t4, -16($fp)
	b	label_6
label_8:
	li	$t4, 1
	lw	$t3, -12($fp)
	lw	$t2, -20($fp)
	bne	$t3, $t2, label_17
label_35:
	b	label_18
label_18:
	li	$t4, 0
label_17:
	bne	$t4, 0, label_15
label_36:
	b	label_16
label_15:
	lw	$t4, 0($fp)
	lw	$t2, -12($fp)
	mulo	$t3, $t2, 4
	add	$t2, $t4, $t3
	lw	$t3, 0($t2)
	move	$t2, $t3
	sw	$t2, -24($fp)
	lw	$t2, 0($fp)
	lw	$t3, -20($fp)
	mulo	$t4, $t3, 4
	add	$t3, $t2, $t4
	lw	$t4, 0($t3)
	move	$t3, $t4
	lw	$t4, 0($fp)
	lw	$t2, -12($fp)
	mulo	$t0, $t2, 4
	add	$t2, $t4, $t0
	sw	$t3, 0($t2)
	lw	$t2, -24($fp)
	move	$t3, $t2
	lw	$t2, 0($fp)
	lw	$t0, -20($fp)
	mulo	$t4, $t0, 4
	add	$t0, $t2, $t4
	sw	$t3, 0($t0)
label_16:
	lw	$t0, -12($fp)
	add	$t3, $t0, 1
	move	$t0, $t3
	sw	$t0, -12($fp)
	b	label_1
label_3:
	li	$v0, 0
	lw	$t0, -8($fp)
	move	$ra, $t0
	lw	$t0, -4($fp)
	move	$t3, $t0
	move	$sp, $fp
	move	$fp, $t3
	jr	$ra

	.text
	.globl	main
main:
	subu	$sp, $sp, 148
	sw	$fp, 144($sp)
	add	$fp, $sp, 148
	sw	$ra, -8($fp)
	la	$t3, str_1
	move	$t0, $t3
	li	$v0, 4
	move	$a0, $t0
	syscall
	move	$t3, $t0
	li	$t0, 100
	sw	$t0, -20($fp)
	lw	$t0, -20($fp)
	move	$t3, $t0
	sw	$t3, -16($fp)
	li	$t3, 0
	sw	$t3, -12($fp)
label_19:
	li	$t3, 1
	lw	$t0, -16($fp)
	bgt	$t0, 0, label_22
label_37:
	b	label_23
label_23:
	li	$t3, 0
label_22:
	bne	$t3, 0, label_20
label_38:
	b	label_21
label_20:
	lw	$t3, -16($fp)
	move	$t0, $t3
	la	$t3, glob_a
	lw	$t4, -12($fp)
	mulo	$t2, $t4, 4
	add	$t4, $t3, $t2
	sw	$t0, 0($t4)
	lw	$t4, -12($fp)
	add	$t0, $t4, 1
	move	$t4, $t0
	sw	$t4, -12($fp)
	lw	$t4, -16($fp)
	sub	$t0, $t4, 1
	move	$t4, $t0
	sw	$t4, -16($fp)
	b	label_19
label_21:
	la	$t4, glob_a
	move	$t0, $t4
	lw	$t4, -20($fp)
	move	$t2, $t4
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
	sw	$t0, 0($sp)
	sw	$t2, 4($sp)
	jal	func_selectSort
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
	move	$t2, $v0
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
	move	$t0, $t2
	li	$t2, 0
	sw	$t2, -16($fp)
label_24:
	li	$t2, 1
	lw	$t0, -16($fp)
	lw	$t4, -20($fp)
	blt	$t0, $t4, label_27
label_39:
	b	label_28
label_28:
	li	$t2, 0
label_27:
	bne	$t2, 0, label_25
label_40:
	b	label_26
label_25:
	la	$t2, str_2
	move	$t4, $t2
	lw	$t2, -16($fp)
	move	$t0, $t2
	la	$t2, str_3
	move	$t3, $t2
	la	$t2, glob_a
	lw	$t1, -16($fp)
	mulo	$t5, $t1, 4
	add	$t1, $t2, $t5
	lw	$t5, 0($t1)
	move	$t1, $t5
	la	$t5, str_4
	move	$t2, $t5
	li	$v0, 4
	move	$a0, $t4
	syscall
	move	$t5, $t4
	li	$v0, 1
	move	$a0, $t0
	syscall
	move	$t4, $t0
	li	$v0, 4
	move	$a0, $t3
	syscall
	move	$t0, $t3
	li	$v0, 1
	move	$a0, $t1
	syscall
	move	$t3, $t1
	li	$v0, 4
	move	$a0, $t2
	syscall
	move	$t1, $t2
	lw	$t2, -16($fp)
	add	$t1, $t2, 1
	move	$t2, $t1
	sw	$t2, -16($fp)
	b	label_24
label_26:
	li	$v0, 0
	lw	$t2, -8($fp)
	move	$ra, $t2
	lw	$t2, -4($fp)
	move	$t1, $t2
	move	$sp, $fp
	move	$fp, $t1
	jr	$ra
