
	.rdata
str_3:
	.asciiz	"\n"

	.rdata
str_2:
	.asciiz	" e "

	.rdata
str_1:
	.asciiz	"Fibonnaci de "

	.rdata
str_0:
	.asciiz	"\n"

	.text
	.globl	func_fibonacci
func_fibonacci:
	subu	$sp, $sp, 132
	sw	$fp, 128($sp)
	add	$fp, $sp, 132
	sw	$ra, -8($fp)
	li	$t0, 1
	li	$t1, 1
	lw	$t2, 0($fp)
	beq	$t2, 1, label_7
label_11:
	b	label_8
label_8:
	li	$t1, 0
label_7:
	bne	$t1, 1, label_4
label_12:
	b	label_5
label_4:
	li	$t1, 1
	lw	$t2, 0($fp)
	beq	$t2, 2, label_9
label_13:
	b	label_10
label_10:
	li	$t1, 0
label_9:
	bne	$t1, 1, label_6
label_14:
	b	label_5
label_6:
	li	$t0, 0
label_5:
	bne	$t0, 0, label_1
label_15:
	b	label_2
label_1:
	li	$v0, 1
	lw	$t0, -8($fp)
	move	$ra, $t0
	lw	$t0, -4($fp)
	move	$t1, $t0
	move	$sp, $fp
	move	$fp, $t1
	jr	$ra
	b	label_3
label_2:
	lw	$t1, 0($fp)
	sub	$t0, $t1, 1
	move	$t1, $t0
	sw	$t0, -16($fp)
	sw	$t1, -20($fp)
	sw	$t2, -24($fp)
	sw	$t3, -28($fp)
	sw	$t4, -32($fp)
	sw	$t5, -36($fp)
	sw	$t6, -40($fp)
	sw	$t7, -44($fp)
	sw	$t8, -48($fp)
	sw	$t9, -52($fp)
	sw	$v0, -56($fp)
	sw	$v1, -60($fp)
	sw	$a0, -64($fp)
	sw	$a1, -68($fp)
	sw	$a2, -72($fp)
	sw	$a3, -76($fp)
	sw	$s0, -80($fp)
	sw	$s1, -84($fp)
	sw	$s2, -88($fp)
	sw	$s3, -92($fp)
	sw	$s4, -96($fp)
	sw	$s5, -100($fp)
	sw	$s6, -104($fp)
	sw	$s7, -108($fp)
	sw	$t1, 0($sp)
	jal	func_fibonacci
	lw	$t0, -16($fp)
	lw	$t1, -20($fp)
	lw	$t2, -24($fp)
	lw	$t3, -28($fp)
	lw	$t4, -32($fp)
	lw	$t5, -36($fp)
	lw	$t6, -40($fp)
	lw	$t7, -44($fp)
	lw	$t8, -48($fp)
	lw	$t9, -52($fp)
	move	$t1, $v0
	lw	$v0, -56($fp)
	lw	$v1, -60($fp)
	lw	$a0, -64($fp)
	lw	$a1, -68($fp)
	lw	$a2, -72($fp)
	lw	$a3, -76($fp)
	lw	$s0, -80($fp)
	lw	$s1, -84($fp)
	lw	$s2, -88($fp)
	lw	$s3, -92($fp)
	lw	$s4, -96($fp)
	lw	$s5, -100($fp)
	lw	$s6, -104($fp)
	lw	$s7, -108($fp)
	move	$t0, $t1
	move	$t1, $t0
	lw	$t0, 0($fp)
	sub	$t2, $t0, 2
	move	$t0, $t2
	move	$t2, $t1
	sw	$t0, -16($fp)
	sw	$t1, -20($fp)
	sw	$t2, -24($fp)
	sw	$t3, -28($fp)
	sw	$t4, -32($fp)
	sw	$t5, -36($fp)
	sw	$t6, -40($fp)
	sw	$t7, -44($fp)
	sw	$t8, -48($fp)
	sw	$t9, -52($fp)
	sw	$v0, -56($fp)
	sw	$v1, -60($fp)
	sw	$a0, -64($fp)
	sw	$a1, -68($fp)
	sw	$a2, -72($fp)
	sw	$a3, -76($fp)
	sw	$s0, -80($fp)
	sw	$s1, -84($fp)
	sw	$s2, -88($fp)
	sw	$s3, -92($fp)
	sw	$s4, -96($fp)
	sw	$s5, -100($fp)
	sw	$s6, -104($fp)
	sw	$s7, -108($fp)
	sw	$t0, 0($sp)
	jal	func_fibonacci
	lw	$t0, -16($fp)
	lw	$t1, -20($fp)
	lw	$t2, -24($fp)
	lw	$t3, -28($fp)
	lw	$t4, -32($fp)
	lw	$t5, -36($fp)
	lw	$t6, -40($fp)
	lw	$t7, -44($fp)
	lw	$t8, -48($fp)
	lw	$t9, -52($fp)
	move	$t0, $v0
	lw	$v0, -56($fp)
	lw	$v1, -60($fp)
	lw	$a0, -64($fp)
	lw	$a1, -68($fp)
	lw	$a2, -72($fp)
	lw	$a3, -76($fp)
	lw	$s0, -80($fp)
	lw	$s1, -84($fp)
	lw	$s2, -88($fp)
	lw	$s3, -92($fp)
	lw	$s4, -96($fp)
	lw	$s5, -100($fp)
	lw	$s6, -104($fp)
	lw	$s7, -108($fp)
	move	$t1, $t0
	add	$t0, $t2, $t1
	move	$v0, $t0
	lw	$t0, -8($fp)
	move	$ra, $t0
	lw	$t0, -4($fp)
	move	$t1, $t0
	move	$sp, $fp
	move	$fp, $t1
	jr	$ra
label_3:
	li	$v0, 0
	lw	$t1, -8($fp)
	move	$ra, $t1
	lw	$t1, -4($fp)
	move	$t0, $t1
	move	$sp, $fp
	move	$fp, $t0
	jr	$ra

	.text
	.globl	main
main:
	subu	$sp, $sp, 136
	sw	$fp, 132($sp)
	add	$fp, $sp, 136
	sw	$ra, -8($fp)
	la	$t0, str_0
	move	$t1, $t0
	li	$v0, 4
	move	$a0, $t1
	syscall
	move	$t0, $t1
	li	$t1, 50
	sw	$t1, -12($fp)
	la	$t1, str_1
	move	$t0, $t1
	lw	$t1, -12($fp)
	move	$t2, $t1
	la	$t1, str_2
	move	$t3, $t1
	lw	$t1, -12($fp)
	move	$t4, $t1
	sw	$t0, -20($fp)
	sw	$t1, -24($fp)
	sw	$t2, -28($fp)
	sw	$t3, -32($fp)
	sw	$t4, -36($fp)
	sw	$t5, -40($fp)
	sw	$t6, -44($fp)
	sw	$t7, -48($fp)
	sw	$t8, -52($fp)
	sw	$t9, -56($fp)
	sw	$v0, -60($fp)
	sw	$v1, -64($fp)
	sw	$a0, -68($fp)
	sw	$a1, -72($fp)
	sw	$a2, -76($fp)
	sw	$a3, -80($fp)
	sw	$s0, -84($fp)
	sw	$s1, -88($fp)
	sw	$s2, -92($fp)
	sw	$s3, -96($fp)
	sw	$s4, -100($fp)
	sw	$s5, -104($fp)
	sw	$s6, -108($fp)
	sw	$s7, -112($fp)
	sw	$t4, 0($sp)
	jal	func_fibonacci
	lw	$t0, -20($fp)
	lw	$t1, -24($fp)
	lw	$t2, -28($fp)
	lw	$t3, -32($fp)
	lw	$t4, -36($fp)
	lw	$t5, -40($fp)
	lw	$t6, -44($fp)
	lw	$t7, -48($fp)
	lw	$t8, -52($fp)
	lw	$t9, -56($fp)
	move	$t4, $v0
	lw	$v0, -60($fp)
	lw	$v1, -64($fp)
	lw	$a0, -68($fp)
	lw	$a1, -72($fp)
	lw	$a2, -76($fp)
	lw	$a3, -80($fp)
	lw	$s0, -84($fp)
	lw	$s1, -88($fp)
	lw	$s2, -92($fp)
	lw	$s3, -96($fp)
	lw	$s4, -100($fp)
	lw	$s5, -104($fp)
	lw	$s6, -108($fp)
	lw	$s7, -112($fp)
	move	$t1, $t4
	move	$t4, $t1
	la	$t1, str_3
	move	$t5, $t1
	li	$v0, 4
	move	$a0, $t0
	syscall
	move	$t1, $t0
	li	$v0, 1
	move	$a0, $t2
	syscall
	move	$t0, $t2
	li	$v0, 4
	move	$a0, $t3
	syscall
	move	$t2, $t3
	li	$v0, 1
	move	$a0, $t4
	syscall
	move	$t3, $t4
	li	$v0, 4
	move	$a0, $t5
	syscall
	move	$t4, $t5
	li	$v0, 0
	lw	$t5, -8($fp)
	move	$ra, $t5
	lw	$t5, -4($fp)
	move	$t4, $t5
	move	$sp, $fp
	move	$fp, $t4
	jr	$ra
