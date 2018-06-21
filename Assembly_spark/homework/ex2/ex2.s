fmt0: .asciz "%d %d"
fmt1: .asciz "%d\n"
fmt2: .asciz "quotient : %d , remainder : %d\n"
fmt3: .asciz "---------------START PRINT--------------\n"	
							! %l0 : x, %l1 : y, %l2 : temp, %l3 : const_x, %l4 : count

	.align 4
	.global	main, scanf, printf
main:	save 	%sp, -104, %sp
	set 	fmt0, %o0
	add	%fp, -4, %o1
	add 	%fp, -8, %o2
	call 	scanf
	nop
	
	ld 	[%fp-4], %l0
	ld 	[%fp-8], %l1
	cmp 	%l0, %l1
	bg 	X_IS_BIGGER_THAN_Y
	nop
	bne 	SWAP		! branch when X is smaller than Y, Not equal to
	nop
EXIT:	set	fmt1, %o0	! if X is equal to Y, then Quit the Program
	mov	0, %o1
	call	printf
	nop
	mov	1, %g1
	ta	0

SWAP:	mov	%l1, %l2	! swapping X and Y. X must be bigger than Y
	mov	%l0, %l1
	mov	%l2, %l0

X_IS_BIGGER_THAN_Y:
	mov 	%l0, %l3 
	clr 	%l4
	ba 	ODD_CNT
	cmp	%l0,	%l1

ODD_CNT_LOOP:
	be	EVEN_CASE
	add	%l0, -1, %l0
	inc	%l4
EVEN_CASE:
	cmp	%l0, %l1
ODD_CNT:bge,a	ODD_CNT_LOOP
	btst	1, %l0	
	cmp	%l4, 2
	bl	EXIT
	nop

X_BOUNDARY:
	btst	1, %l3
	be,a	Y_BOUNDARY
	add	%l3, -1, %l0
	mov	%l3, %l0
Y_BOUNDARY:
	btst	1, %l1
	be,a	Y_IS_EVEN
	add	%l1, 1, %l1

Y_IS_EVEN:
	mov	%l0, %l3
	set	fmt3,	%o0
	call	printf
	clr	%l4

	ba	PRINT
	cmp	%l0, %l1
PRINT_LOOP:
	set	fmt1, %o0
	mov	%l0, %o1
	call	printf
	add	%l0, -2, %l0
	cmp	%l0, %l1
PRINT:	bge	PRINT_LOOP
	nop

	ba	DIV
	cmp	%l3, %l1
DIV_LOOP:
	sub	%l3, %l1, %l3
	cmp	%l3, %l1
DIV:	bge,a	DIV_LOOP
	inc	%l4

	set	fmt2, %o0
	mov	%l4, %o1
	mov	%l3, %o2
	call 	printf
	nop
	mov	1, %g1
	ta	0
