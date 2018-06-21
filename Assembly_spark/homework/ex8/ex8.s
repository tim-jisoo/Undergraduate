	BUFFER = 1024
	READ = 3
	WRITE = 4
	CREATE = 8

fmt0:	.asciz	"foo"
	.align 4	
	.global main
main:	save 	%sp, (-BUFFER-92)&-8, %sp
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
create:	set	fmt0, %o0
	mov	0600, %o1
	mov	CREATE, %g1
	ta	0
	addcc	%o0,	0, %l0		!%l0 is file discripter
	bl	error
	nop	
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
read:	mov	0, %o0
	sub	%fp, BUFFER, %o1
	mov	BUFFER, %o2
	mov	READ, %g1
	ta	0
	addcc	%o0, 0, %l1		!%l1 is # of character which is read
	bl	error		
	nop
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	sub	%fp, BUFFER, %l2 	!%l2 is address of string which is stored in stack	
Loop:	ldub	[%l2], %l3		!%l3 is character of string
	cmp	%l3, '0'
	be	EXIT
	nop
	cmp	%l3, 'A'
	bne	Keep_going
	nop
CHANGE:	add	%l3, 1, %l3			
	stb	%l3, [%l2]
Keep_going:
	cmp	%l3, 0
	bne,a	Loop
	inc	%l2
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
write:	mov	1, %o0
	sub	%fp, BUFFER, %o1
	mov	%l1, %o2
	mov	WRITE, %g1
	ta	0
	cmp	%o0, %l0
	bl	error
	nop
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!	
write_f:
	mov	%l0, %o0
	sub	%fp, BUFFER, %o1
	mov	%l1,  %o2
	mov	WRITE, 	%g1
	ta	0
	
	ba	read
	nop 
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
EXIT:	mov	0, %o0
	mov	1, %g1
	ta	0	
error:	mov	1, %o0	
	clr	%g1	
	ta	0
	
			

