	.section	".data"
str1:	.skip	100
str2:	.skip	100

	.section	".text"
fmt0:	.asciz "%s"
fmt1:	.asciz "%s\n"
fmt2:	.asciz "YES\n"
fmt3:	.asciz "NO\n"
	.align 4	
	.global main, scanf, printf
main:	save	%sp, -240, %sp
	set	fmt0, %o0
	set	str1, %o1
	call	scanf
	nop

	set	fmt0, %o0
	set	str2, %o1
	call	scanf
	nop

	set	fmt1, %o0	
	set	str1, %o1
	call	printf
	nop 

	set	fmt1, %o0
	set	str2, %o1
	call	printf
	nop
					! %l1 : str1의 주소, %l2 : str2의 주소
	set	str1, %l1
	set	str2, %l2	
					! %l3 : str1의 길이, %l4 : str2의 길이
	clr	%l3
str_len_1:
	ldub	[%l1], %o0
	nop
	add	%l1, 1, %l1
	cmp	%o0, 0
	bne,a	str_len_1
	inc	%l3

	clr	%l4
str_len_2:
	ldub	[%l2], %o0
	nop
	add	%l2, 1, %l2
	cmp	%o0, 0
	bne,a	str_len_2
	inc	%l4

	set	str1, %l1
	set	str2, %l2

	cmp %l3, %l4
	bl	MATCHING_TEST_START
	nop
			

				! %l1에 저장된 문자열의 길이가 무조건 짧게 만들어준다. 그리고, 가장짧은 문자열의 길이는 %l3에 저장한다.
BIGGER:				! 그러니까 %l2의 길이가 더긴것,	
	set	str1, %l2
	set	str2, %l1	
	mov	%l4, %o0
	mov	%l3, %l4
	mov	%o0, %l3
				 
LESS:					
MATCHING_TEST_START:
	sub	%l4, %l3, %l4	! %l0에 가장짧은 스트링과 가장 긴스트링을 뺀 절대값이 저장된다. 	
	mov	0, %l0		! %l0는	 긴string에 사용할 index이다. 초기값 0임. 
	mov	0, %o4		! %o4는  짧은 string에 사용할 index이다. 초기값 0임.	

	ba	OUTTER_LOOP
	nop

INNER_LOOP:
	cmp	%o4, %l3
	bge,a 	OUTTER_LOOP
	inc 	%l0

	add	%l2, %l0, %o3
	add	%o3, %o4, %o3
	ldub	[%o3], %o3
	add	%l1, %o4, %o2
	ldub	[%o2], %o2	
	cmp	%o3, %o2
	bne,a	OUTTER_LOOP
	inc 	%l0

YES_TEST:
	add	%l3, -1, %o0
	cmp	%o4, %o0
	bne,a	INNER_LOOP
	inc 	%o4			

	ba	YES
	nop


OUTTER_LOOP:
	cmp	%l0, %l4
	ble	INNER_LOOP
	nop		

NO:
	set	fmt3, %o0
	call	printf
	nop
	ba	EXIT
	nop

YES:	
	set	fmt2, %o0
	call	printf
	nop

EXIT:	mov 	1, %g1
	ta	0






