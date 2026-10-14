	.text

strcpy:
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	push	17,010
%L2:
	move	1,011
	ibp	011
	ldb	2,1
	move	3,010
	ibp	010
	dpb	2,3
	jumpn	2,%L2
	move	1,0(17)
%L1:
	move	10,-2(17)
	move	11,-1(17)
	SUB	17,[3,,3]
	popj	17,

strncpy:
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	push	17,012
	move	12,3
	push	17,010
%L4:
	move	1,012
	jumpe	1,%L5
	move	2,011
	ldb	3,2
	jumpe	3,%L5
	move	4,011
	ibp	011
	ldb	5,4
	move	6,010
	ibp	010
	dpb	5,6
	soja	12,%L4
%L5:
%L6:
	move	1,012
	subi	12,1
	jumpe	1,%L7
	setz	2,
	move	3,010
	ibp	010
	dpb	2,3
	jrst	%L6
%L7:
	move	1,0(17)
%L3:
	move	10,-3(17)
	move	11,-2(17)
	move	12,-1(17)
	SUB	17,[4,,4]
	popj	17,

strcat:
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	push	17,010
%L9:
	move	1,010
	ldb	2,1
	jumpe	2,%L10
	ibp	010
	jrst	%L9
%L10:
%L11:
	move	1,011
	ibp	011
	ldb	2,1
	move	3,010
	ibp	010
	dpb	2,3
	jumpn	2,%L11
	move	1,0(17)
%L8:
	move	10,-2(17)
	move	11,-1(17)
	SUB	17,[3,,3]
	popj	17,

strncat:
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	push	17,012
	move	12,3
	push	17,010
%L13:
	move	1,010
	ldb	2,1
	jumpe	2,%L14
	ibp	010
	jrst	%L13
%L14:
%L15:
	move	1,012
	subi	12,1
	jumpe	1,%L16
	move	2,011
	ldb	3,2
	jumpe	3,%L16
	move	4,011
	ibp	011
	ldb	5,4
	move	6,010
	ibp	010
	dpb	5,6
	jrst	%L15
%L16:
	setz	1,
	move	2,010
	dpb	1,2
	move	1,0(17)
%L12:
	move	10,-3(17)
	move	11,-2(17)
	move	12,-1(17)
	SUB	17,[4,,4]
	popj	17,

strchr:
%L17:
	move	3,1
	ldb	4,3
	move	5,2
	andi	5,0777
	camn	4,5
	 popj	17,
	move	6,1
	ibp	1
	ldb	7,6
	jumpn	7,%L17
	setm	1,7
	popj	17,

strrchr:
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	push	17,[0]
%L21:
	move	1,010
	ldb	2,1
	move	3,011
	andi	3,0777
	camn	2,3
	 skipa	4,010
	 trna
	 movem	4,0(17)
	move	5,010
	ibp	010
	ldb	6,5
	jumpn	6,%L21
	move	1,0(17)
%L20:
	move	10,-2(17)
	move	11,-1(17)
	SUB	17,[3,,3]
	popj	17,

	.bss

%L24:
	.space 4

	.text

strtok:
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	skipe	1,010
	 movem	10,%L24
	ADD	17,[1,,1]
	skipe	3,%L24
	 jrst	%L27
	setm	1,3
	jrst	%L25
%L27:
%L28:
	ldb	1,%L24
	jumpe	1,%L29
	ldb	2,%L24
	move	1,011
	pushj	17,strchr
	jumpe	1,%L29
	ibp	%L24
	move	2,%L24
	jrst	%L28
%L29:
	ldb	1,%L24
	jumpn	1,%L30
	setm	2,1
	movem	2,%L24
	setz	1,
	jrst	%L25
%L30:
	move	2,%L24
	movem	2,0(17)
%L31:
	ldb	1,%L24
	jumpe	1,%L32
	ldb	2,%L24
	move	1,011
	pushj	17,strchr
	jumpn	1,%L32
	ibp	%L24
	move	2,%L24
	jrst	%L31
%L32:
	ldb	1,%L24
	jumpe	1,%L34
	setz	2,
	move	4,%L24
	ibp	%L24
	dpb	2,4
	jrst	%L33
%L34:
	setzb	1,%L24
%L33:
	move	1,0(17)
%L25:
	move	10,-2(17)
	move	11,-1(17)
	SUB	17,[3,,3]
	popj	17,

strcspn:
	push	17,016
	push	17,010
	move	10,1
	push	17,011
	move	11,2
	ADD	17,[2,,2]
	movem	10,-1(17)
%L36:
	ldb	1,-1(17)
	jumpe	1,%L37
	movem	11,0(17)
%L39:
	ldb	3,0(17)
	jumpe	3,%L38
	ldb	2,-1(17)
	came	2,3
	 jrst	%L40
	move	4,-1(17)
	move	1,010
	move	16,1
	ADD	17,[3,,3]
	MOVEM	4,-02(17)
	MOVEM	16,-01(17)
	MOVEM	15,00(17)
	MOVE	5,-02(17)
	ANDI	5,0777777
	MOVE	4,-01(17)
	ANDI	4,0777777
	SUB	5,4
	IMULI	5,04
	HLRZ	15,-02(17)
	LSH	15,-014
	ANDI	15,077
	MOVN	15,15
	ADDI	15,033
	IMULI	15,010
	LSH	15,-06
	ADD	5,15
	HLRZ	15,-01(17)
	LSH	15,-014
	ANDI	15,077
	MOVN	15,15
	ADDI	15,033
	IMULI	15,010
	LSH	15,-06
	SUB	5,15
	MOVE	15,00(17)
	SUB	17,[3,,3]
	move	1,5
	jrst	%L35
%L40:
	ibp	0(17)
	move	1,0(17)
	jrst	%L39
%L38:
	ibp	-1(17)
	move	1,-1(17)
	jrst	%L36
%L37:
	move	2,-1(17)
	move	1,010
	move	16,1
	ADD	17,[3,,3]
	MOVEM	2,-02(17)
	MOVEM	16,-01(17)
	MOVEM	15,00(17)
	MOVE	3,-02(17)
	ANDI	3,0777777
	MOVE	2,-01(17)
	ANDI	2,0777777
	SUB	3,2
	IMULI	3,04
	HLRZ	15,-02(17)
	LSH	15,-014
	ANDI	15,077
	MOVN	15,15
	ADDI	15,033
	IMULI	15,010
	LSH	15,-06
	ADD	3,15
	HLRZ	15,-01(17)
	LSH	15,-014
	ANDI	15,077
	MOVN	15,15
	ADDI	15,033
	IMULI	15,010
	LSH	15,-06
	SUB	3,15
	MOVE	15,00(17)
	SUB	17,[3,,3]
	move	1,3
%L35:
	move	10,-3(17)
	move	11,-2(17)
	move	16,-4(17)
	SUB	17,[5,,5]
	popj	17,


	.globl strcpy
	.globl strncpy
	.globl strcat
	.globl strncat
	.globl strchr
	.globl strrchr
	.globl strtok
	.globl strcspn
