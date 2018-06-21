fmt0:	.asciz "%s"
fmt1:	.asciz "%s\n"
fmt2:	.asciz "%d\n"
	.align 4
	.global main, scanf, printf

main:	save 	%sp, -112, %sp
	set 	fmt0, %o0
	add 	%fp, -16, %o1
	call	scanf
	nop

	set	fmt1, %o0
	add	%fp, -16, %o1	
	call	printf
	nop

	add 	%fp, -16, %o0
	call	foo
	nop

	set	fmt1, %o0
	add	%fp, -16, %o1
	call	printf
	nop

EXIT:	mov	1, %g1
	ta 0

foo:	save	%sp, -120, %sp
	mov	%i0, %l0
	mov	0, %l1
loop:	add	%l0, %l1, %l2
	ldub	[%l2], %l3
	add	%l3, 32, %l4
	!%l4 is 
	cmp	%l4, 'z'
	bg	SMALL_ALPHA
	nop
	ba	INC
	nop
SMALL_ALPHA:
	sub	%l3, 32, %l3	
	stb	%l3, [%l2]
INC:	inc	%l1
	cmp	%l1, 15
	bne	loop
	nop
	ret
	restore	

		

	
