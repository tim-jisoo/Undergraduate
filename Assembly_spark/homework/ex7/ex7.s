fmt0:	.asciz "%d"
fmt1: 	.asciz "%d\n"
fmt2:	.asciz "the result of least multiple value is : "
	.align 4
	.global main, scanf, printf

main:	save	%sp, -96, %sp	
	add	%fp, -4, %l2	!%l2 is address of N, N is in the stack (%fp-4)
	set	fmt0, %o0
	mov	%l2, %o1	
	call	scanf
	nop
	
	ld	[%l2], %l0	!%l0 is N
	mov	20, %l3	
	cmp	%l3, %l0		!if(N>=20) 
	ble	EXIT
	nop

	mov	%l0, %o0
	mov	-4, %o1
	call	.mul
	nop			!the result of .mul is N*(-4) which is stored in %o0
	add	%sp, %o0, %sp	!additional stack frame allocation

	add	%l2, -4, %l4 
	add	%sp, 92, %l1	!%l1 is address of parameter       
SCANF_Loop:
	set	fmt0, %o0
	mov	%l1, %o1
	call	scanf
	nop
	cmp	%l1, %l4		
	bl,a	SCANF_Loop
	add	%l1, 4, %l1

	mov	%l2, %o0
	call	ssort
	nop

	add	%l2, -4, %l4
	add	%sp, 92, %l1
PRINTF_Loop:
	set	fmt1, %o0
	ld	[%l1], %o1
	call	printf
	nop
	cmp	%l1, %l4	
	bl,a	PRINTF_Loop
	add	%l1, 4, %l1

EXIT:	mov	1, %g1
	ta	0


ssort:
	save	%sp, -96, %sp	
ssort_Init:
	mov	%i0, %l2	!%l2 is pointer which is address of N of stack , size = sizeof(arr)	
OUTTER_Loop_Init:
	add	%fp, 92, %l0	!%l0 is pointer which is address of integer of stack, i = 0;	
OUTTER_Loop:
	cmp	%l0, %l2	!i<sizeof(arr)
	bge	END_Of_ssort
	nop		
INNER_Loop_Init:
	add	%l0, 4, %l1	!j=i+1	%l1 is j	
INNER_Loop:
	cmp	%l1, %l2	!j<sizeof(arr)
	bge,a	OUTTER_Loop	
	add	%l0, 4, %l0	!i++
	
	ld	[%l0] ,%l3	!%l3 is arr[i]
	ld	[%l1] ,%l4	!%l4 is arr[j]
	cmp	%l3, %l4	!if(arr[i]>arr[j])
	ble,a	INNER_Loop			
	add	%l1, 4, %l1	!j++	
	
	mov	%l3, %l5	!%l5 is temp	, temp = arr[i];	
	st	%l4, [%l0]	!arr[i] = arr[j];
	st	%l5, [%l1]	!arr[j] = temp;		
	ba	INNER_Loop
	add	%l1, 4, %l1
	
END_Of_ssort:
	set	fmt2, %o0
	call	printf
	nop	
	
	ld	[%fp+92], %o0
	ld	[%fp+96], %o1
	call	.mul
	nop
	mov	%o0, %o1
	set	fmt1, %o0
	call 	printf
	nop	
	ret	
	restore










