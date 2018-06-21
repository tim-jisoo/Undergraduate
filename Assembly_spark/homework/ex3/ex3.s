fmt0:	.asciz "%d"
fmt1:	.asciz "%d\n"
fmt2:	.asciz "put integer value (maximum 30, next step:0)\n"
fmt3:	.asciz "put num what you wanna see up to\n"
fmt4:	.asciz "the integer value is...\n"
fmt5:	.asciz "error occured...\n"
	.align	4
	.global	main, scanf, printf

main:	save	%sp, (-64-(4*31))&-8, %sp

	mov	0, %l4
	mov	-4, %l3

	set fmt2, %o0
	call printf
	nop

loop1:	cmp	%l4, 30
	bge	flow
	nop
	
	set	fmt0, %o0
	add	%fp, %l3, %o1
	call	scanf
	nop

	add	%fp, %l3, %l0
	ld	[%l0], %l1

	tst	%l1
	bz	flow
	nop

	inc	%l4
	add	%l3, -4, %l3
	
	ba loop1
	nop

flow:	set	fmt3, %o0
	call	printf
	nop

	set	fmt0, %o0
	add	%fp, %l3, %o1
	call scanf
	nop

	add	%fp, %l3, %l0
	ld	[%l0], %l2
	add	%l3, 4, %l3

	mov	-4, %o0
	call	.mul
	mov	%l2, %o1

	mov	%o0, %l1

	add	%fp, %l1, %l3

	cmp	%l4, 0
	be	EXIT
	nop

	cmp	%l2, %l4
	bg	EXIT
	nop

	set	fmt4, %o0
	call	printf
	nop

loop2:	cmp	%l3, %fp
	be	flow2
	nop

	set	fmt1, %o0
	ld	[%l3],	%o1
	call	printf
	nop
	
	add	%l3, 4, %l3
	ba	loop2
	nop

flow2:
	mov	1, %g1
	ta	0

EXIT:
	set	fmt5, %o0
	call	printf
	nop
		
	set	fmt1, %o0
	mov	0, %o1
	call	printf
	nop
	
	mov	1, %g1
	ta	0	
















