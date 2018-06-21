fmt0:	.asciz "%s"
fmt1:	.asciz "%s\n"
fmt2:	.asciz "%d\n"
	.align 4
	.global main, scanf, printf

main:	save %sp, -112, %sp
	set	fmt0, %o0
	add	%fp, -16, %o1
	call	scanf
	nop

	add	%fp, -16, %l0
	set	fmt1, %o0
	mov	%l0, %o1
	call	printf	
	nop

	clr	%l4
COUNT:	ldub	[%l0], %l2
	add	%l0, 1, %l0	
	cmp	%l2, 0
	bne,a	COUNT
	inc %l4	

	set	fmt2, %o0
	mov	%l4, %o1
	call	printf
	nop	

	add	%fp, -16, %l0
COMPARE:
	ldub	[%l0], %l2
	add	%l0, 1, %l0	
	cmp	%l2, 'a'
	bne	NO
	nop
	
	ldub	[%l0], %l2
	add	%l0, 1, %l0
	cmp	%l2, 'b'
	bne	NO
	nop

	ldub	[%l0], %l2
	add	%l0, 1, %l0
	cmp	%l2, 'c'
	bne	NO
	nop	
	

	set	fmt1, %o0
	mov	%l0,	%o1
	call	printf
	nop	
NO:		 
	mov	1, %g1
	ta 0
