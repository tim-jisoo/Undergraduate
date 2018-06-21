fmt0:	.asciz "%lf"
fmt1:	.asciz "%lf\n"
	.align 4
	.global main, scanf, printf
main:	save 	%sp,(-96-8*11)&-8, %sp
	add	%fp,-8*10, %l0
	add	%fp, -8, %l1 
SCANF_LOOP:
	set	fmt0, %o0
	mov	%l1, %o1	
	call	scanf
	nop
	cmp	%l1, %l0
	bg,a	SCANF_LOOP			
	add	%l1, -8, %l1

function_minSTART:
	
	add	%fp, -8, %l0
	ldd	[%l0], %f0	!min is %f0
	add	%l0, -8, %l0	
loop:	ldd	[%l0], %f2
	fcmpd	%f0, %f2	
	nop
	fble	NORMAL	
	nop
CHANGE:	fmovs	%f2, %f0
	fmovs	%f3, %f1
NORMAL:	cmp	%l0, %l1	
	bg	loop	
	add	%l0, -8, %l0	!final fmin is stroed in top of stack (%fp-8*11)	
	
	std	%f0, [%l0] 	
	ldd	[%l0], %l6	
	set	fmt1, %o0	
	mov	%l6, %o1
	mov	%l7, %o2	
call	printf
	nop	

	mov	1, %g1
	ta	0	
