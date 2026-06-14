#define LABEL_PREFIX	%L
#define CONCAT1(a, b)	CONCAT2(a, b)
#define CONCAT2(a, b)	a ## b
#define FUNC(x)		CONCAT1 (UNDERSCORE, x)
#define L(x)		CONCAT2 (LABEL_PREFIX, x)
#ifdef PDP10_DOBJ
#define GLOBAL(x)	.globl x
#else
#define GLOBAL(x)	ENTRY x
#endif
#define RETURN		POPJ 17,
#define SECTION_RO(x)	.PSECT x/ronly
#define SECTION_RW(x)	.PSECT x/rwrite
#define SECTION_END	.ENDPS
#define FILE_START(x)	TITLE x
#define FILE_END	END

	FILE_START (libgcc1)

	SECTION_RO (.text)

/* The sequences for udivsi3 and umodsi3 were taken from KCC and are
   derived from one suggested by Peter Samson at Systems Concepts.  */

#ifdef L_udivsi3
	GLOBAL(FUNC(udivsi3))
FUNC(udivsi3):
#if __REGPARM__ == 0
	MOVE 1,-1(17)
#endif
#if __REGPARM__ >= 2
	SKIPGE 3,2
#else
	SKIPGE 3,-2+__REGPARM__(17)
#endif
	 JRST %L1
	JUMPGE 1,%L3
	CAIG 3,1
	 JRST %L2
	MOVE 2,1
	MOVEI 1,1
	DIV 1,3
	RETURN
%L1:	MOVE 2,1
	MOVEI 1,0
	JUMPGE 2,%L4
	CAMGE 2,3
	 RETURN
	SUB 2,3
	AOJA 1,%L4
%L2:	TDZA 2,2
%L3:	IDIV 1,3
%L4:	RETURN
#endif

#if 0
#if __REGMARM__ >= 2
	MOVE 3,2
	MOVE 2,1
	SKIPGE 1,3
	 JRST %L1
	SOJN 1,%L2
	MOVE 1,2
	RETURN
%L1:	MOVEI 1,0
	JUMPGE 2,%L3
	CAMGE 2,3
	 RETURN
	SUB 2,3
	AOJA 1,%L3
%L2:	TLNN 2,400000
	 TDZA 1,1
	  MOVEI 1,1
	DIV 1,3
%L3:	RETURN
#endif
#if __REGPARM__ == 0
	MOVE 2,-1(17)
	SKIPGE 1,-2(17)
	 JRST %L1
	SOJN 1,%L2
	MOVE 1,2
	RETURN
%L1:	MOVEI 1,0
	JUMPGE 2,%L3
	CAMGE 2,-2(17)
	 RETURN
	SUB 2,-2(17)
	AOJA 1,%L3
%L2:	TLNN 2,400000
	 TDZA 1,1
	  MOVEI 1,1
	DIV 1,-2(17)
%L3:	RETURN
#endif
#endif

#ifdef L_umodsi3
	GLOBAL(FUNC(umodsi3))
FUNC(umodsi3):
#if __REGPARM__ == 0
	MOVE 1,-1(17)
#endif
#if __REGPARM__ >= 2
	SKIPGE 3,2
#else
	SKIPGE 3,-2+__REGPARM__(17)
#endif
	 JRST %L1
	JUMPGE 1,%L3
	CAIG 3,1
	 JRST %L2
	MOVE 2,1
	MOVEI 1,1
	DIV 1,3
	JRST %L4
%L1:	MOVE 2,1
	MOVEI 1,0
	JUMPGE 2,%L4
	CAMGE 2,3
	 JRST %L4
	SUB 2,3
	AOJA 1,%L4
%L2:	TDZA 2,2
%L3:	IDIV 1,3
%L4:	MOVE 1,2
	RETURN
#endif

#if 0
#if __REGPARM__ >= 2
	MOVE 3,2
	MOVE 2,1
	SKIPGE 1,MEM
	JRST $1
	SOJN 1,$2
	RETURN
$1:	MOVEI 1,0
	JUMPGE 2,$3
	CAMGE 2,MEM
	JRST $3
	SUB 2,MEM
	AOJA 1,$3
$2:	TLNN 2,400000
	TDZA 1,1
	MOVEI 1,1
	DIV 1,MEM
$3:	MOVE 1,2
	RETURN

	; 1/ quotient
	; 2/ remainder
#endif
#endif

#ifdef L_adddf3
	GLOBAL(FUNC(adddf3))
FUNC(adddf3):
	HALT
#endif

#ifdef L_subdf3
	GLOBAL(FUNC(subdf3))
FUNC(subdf3):
	HALT
#endif

#ifdef L_negdf2
	GLOBAL(FUNC(negdf2))
FUNC(negdf2):
	HALT
#endif

#ifdef L_muldf3
	GLOBAL(FUNC(muldf3))
FUNC(muldf3):
	HALT
#endif

#ifdef L_divdf3
	GLOBAL(FUNC(divdf3))
FUNC(divdf3):
	HALT
#endif

#ifdef L_eqdf2
	GLOBAL(FUNC(eqdf2))
FUNC(eqdf2):
	HALT
#endif

#ifdef L_nedf2
	GLOBAL(FUNC(nedf2))
FUNC(nedf2):
	HALT
#endif

#ifdef L_gtdf2
	GLOBAL(FUNC(gtdf2))
FUNC(gtdf2):
	HALT
#endif

#ifdef L_gedf2
	GLOBAL(FUNC(gedf2))
FUNC(gedf2):
	HALT
#endif

#ifdef L_ltdf2
	GLOBAL(FUNC(ltdf2))
FUNC(ltdf2):
	HALT
#endif

#ifdef L_ledf2
	GLOBAL(FUNC(ledf2))
FUNC(ledf2):
	HALT
#endif

#ifdef L_fixsfsi
	GLOBAL(FUNC(fixsfsi))
FUNC(fixsfsi):
	HALT
#endif

#ifdef L_fixdfsi
	GLOBAL(FUNC(fixdfsi))
FUNC(fixdfsi):
	HALT
#endif

#ifdef L_floatsisf
	GLOBAL(FUNC(floatsisf))
FUNC(floatsisf):
	HALT
#endif

#ifdef L_floatsidf
	GLOBAL(FUNC(floatsidf))
FUNC(floatsidf):
	HALT
#endif

/* DAIMOS/PDP-10 freestanding libgcc supplement.

   Keep each independently useful helper in a distinct LIB1ASMFUNCS object.
   This lets the archive linker pull only helpers actually referenced by a
   program.  The four DImode division/modulo entry points intentionally remain
   together because they share the same substantial private 72-bit divider.  */

#ifdef L_main
; GCC emits a call to __main at the start of main.  In full hosted GCC this
; supports language runtime initialization such as C++ constructors.  The
; freestanding PDP-10 target has no such runtime yet, so make it a no-op.
	GLOBAL(FUNC(main))
FUNC(main):
	popj 17,
#endif

#ifdef L_divdi3
; Full DImode division/modulo helpers for the freestanding runtime.
;
; ABI used by PDP-10 GCC DImode libcall lowering:
;   input numerator:   AC1 = high word, AC2 = low word
;   input denominator: AC3 = high word, AC4 = low word
;   output value:      AC1 = high word, AC2 = low word
;
; PDP-10 AC numbers in this assembly are octal.  The unsigned core uses:
;   1:2   shifting numerator
;   3:4   denominator
;   5:6   quotient
;   7:10  remainder
;   11    loop counter
;   12    scratch
;   13    boolean flag
;   14    borrow flag
;
; The core is a correctness-first unsigned 72-bit binary long divider.
; Signed wrappers convert operands to absolute values, call the unsigned core,
; and re-apply C signs to quotient/remainder.  It uses only PDP-6/KA10 base
; instructions and no extended addressing.
;
; Register preservation review for the semantic harness:
;   - AC1:AC2 are the defined return pair and are not preserved.
;   - AC3:AC4 are argument/result scratch internally and are not preserved.
;   - AC5..AC14 are saved by each public helper because GCC can keep
;     values live across libcalls in these registers in the bare tests.
;   - AC17 is the stack pointer.  AC0, AC15, and AC16 are not used here.
;
; Division by zero is undefined by C.  The harness deliberately implements
; deterministic benign behavior for direct helper calls: quotient and
; remainder are both returned as zero.  No C testcase should rely on this
; for source-level division by zero.
	.globl	__divdi3
	.globl	__moddi3
	.globl	__udivdi3
	.globl	__umoddi3

__udivdi3:
	push 17,5
	push 17,6
	push 17,7
	push 17,10
	push 17,11
	push 17,12
	push 17,13
	push 17,14
	pushj 17,__p10_udivmoddi4
	pop 17,14
	pop 17,13
	pop 17,12
	pop 17,11
	pop 17,10
	pop 17,7
	pop 17,6
	pop 17,5
	popj 17,

__umoddi3:
	push 17,5
	push 17,6
	push 17,7
	push 17,10
	push 17,11
	push 17,12
	push 17,13
	push 17,14
	pushj 17,__p10_udivmoddi4
	move 1,3
	move 2,4
	pop 17,14
	pop 17,13
	pop 17,12
	pop 17,11
	pop 17,10
	pop 17,7
	pop 17,6
	pop 17,5
	popj 17,

__divdi3:
	push 17,5
	push 17,6
	push 17,7
	push 17,10
	push 17,11
	push 17,12
	push 17,13
	push 17,14
	setz 13,
	tlne 1,400000
	 pushj 17,__p10_abs_num_q
	tlne 3,400000
	 pushj 17,__p10_abs_den_q
	push 17,13
	pushj 17,__p10_udivmoddi4
	pop 17,13
	jumpe 13,__p10_div_restore
	pushj 17,__p10_neg_12
__p10_div_restore:
	pop 17,14
	pop 17,13
	pop 17,12
	pop 17,11
	pop 17,10
	pop 17,7
	pop 17,6
	pop 17,5
	popj 17,

__moddi3:
	push 17,5
	push 17,6
	push 17,7
	push 17,10
	push 17,11
	push 17,12
	push 17,13
	push 17,14
	setz 13,
	tlne 1,400000
	 pushj 17,__p10_abs_num_r
	tlne 3,400000
	 pushj 17,__p10_abs_den_r
	push 17,13
	pushj 17,__p10_udivmoddi4
	pop 17,13
	move 1,3
	move 2,4
	jumpe 13,__p10_mod_restore
	pushj 17,__p10_neg_12
__p10_mod_restore:
	pop 17,14
	pop 17,13
	pop 17,12
	pop 17,11
	pop 17,10
	pop 17,7
	pop 17,6
	pop 17,5
	popj 17,

; Negate AC1:AC2 as a 72-bit two's-complement pair.
__p10_neg_12:
	setca 1,
	setca 2,
	addi 2,1
	jumpn 2,__p10_neg_12_done
	addi 1,1
__p10_neg_12_done:
	popj 17,

; Negate AC3:AC4 as a 72-bit two's-complement pair.
__p10_neg_34:
	setca 3,
	setca 4,
	addi 4,1
	jumpn 4,__p10_neg_34_done
	addi 3,1
__p10_neg_34_done:
	popj 17,

__p10_abs_num_q:
	movei 13,1
	pushj 17,__p10_neg_12
	popj 17,

__p10_abs_den_q:
	xori 13,1
	pushj 17,__p10_neg_34
	popj 17,

__p10_abs_num_r:
	movei 13,1
	pushj 17,__p10_neg_12
	popj 17,

__p10_abs_den_r:
	pushj 17,__p10_neg_34
	popj 17,

; Unsigned divide AC1:AC2 by AC3:AC4.
; Returns quotient in AC1:AC2 and remainder in AC3:AC4.
__p10_udivmoddi4:
	; Division by zero is undefined in C.  Keep harness behavior benign and
	; deterministic: return zero quotient and zero remainder.
	move 12,3
	ior 12,4
	jumpn 12,__p10_udivmod_nonzero
	setz 1,
	setz 2,
	setz 3,
	setz 4,
	popj 17,

__p10_udivmod_nonzero:
	setz 5,             ; quotient high
	setz 6,             ; quotient low
	setz 7,             ; remainder high
	setz 10,            ; remainder low
	movei 11,110        ; 72 bits, octal 110

__p10_udivmod_loop:
	; quotient <<= 1
	lshc 5,1

	; remainder <<= 1, then pull next numerator MSB into remainder bit 0
	setz 13,
	tlne 1,400000
	 movei 13,1
	lshc 7,1
	jumpe 13,__p10_udivmod_no_inbit
	addi 10,1
__p10_udivmod_no_inbit:

	; numerator <<= 1
	lshc 1,1

	; if remainder >= denominator, subtract denominator and set quotient bit.
	pushj 17,__p10_cmp_7_10_ge_3_4
	jumpe 13,__p10_udivmod_no_sub
	pushj 17,__p10_sub_7_10_3_4
	addi 6,1
__p10_udivmod_no_sub:
	sojg 11,__p10_udivmod_loop

	move 1,5
	move 2,6
	move 3,7
	move 4,10
	popj 17,

; Set AC13 = 1 iff unsigned AC7:AC10 >= unsigned AC3:AC4, else 0.
__p10_cmp_7_10_ge_3_4:
	; Compare high words as unsigned 36-bit quantities.
	move 12,7
	xor 12,3
	tlne 12,400000
	 jrst __p10_cmp_hi_sign_diff
	camg 7,3
	 jrst __p10_cmp_hi_not_gt
	movei 13,1
	popj 17,
__p10_cmp_hi_not_gt:
	camn 7,3
	 jrst __p10_cmp_low_words
	jrst __p10_cmp_false
__p10_cmp_low_words:
	; High words equal: compare low words unsigned.
	move 12,10
	xor 12,4
	tlne 12,400000
	 jrst __p10_cmp_lo_sign_diff
	camge 10,4
	 jrst __p10_cmp_false
	movei 13,1
	popj 17,
__p10_cmp_hi_sign_diff:
	tlne 7,400000
	 jrst __p10_cmp_true
	jrst __p10_cmp_false
__p10_cmp_lo_sign_diff:
	tlne 10,400000
	 jrst __p10_cmp_true
	jrst __p10_cmp_false
__p10_cmp_true:
	movei 13,1
	popj 17,
__p10_cmp_false:
	setz 13,
	popj 17,

; AC7:AC10 -= AC3:AC4, treating both as unsigned 72-bit pairs.
; Caller guarantees AC7:AC10 >= AC3:AC4.
__p10_sub_7_10_3_4:
	setz 14,
	; Borrow from low word if unsigned AC10 < unsigned AC4.
	move 12,10
	xor 12,4
	tlne 12,400000
	 jrst __p10_sub_lo_sign_diff
	caml 10,4
	 jrst __p10_sub_no_borrow
	movei 14,1
	jrst __p10_sub_borrow_done
__p10_sub_lo_sign_diff:
	tlne 10,400000
	 jrst __p10_sub_no_borrow
	movei 14,1
	jrst __p10_sub_borrow_done
__p10_sub_no_borrow:
	setz 14,
__p10_sub_borrow_done:
	sub 10,4
	sub 7,3
	jumpe 14,__p10_sub_done
	subi 7,1
__p10_sub_done:
	popj 17,
#endif


	SECTION_END

	SECTION_RO (.rodata)

/* The following tables are used to calculate byte pointer differences.
   They are taken, with small changes, from KCC.  */

#ifdef LBADL6
	GLOBAL(%BADL6)
	040000000005	/* -12. */
	0		/* -11 unused. */
	000000000004	/* -10. */
	200000000004	/* -9. */
	0		/* -8 unused. */
	140000000003	/* -7. */
	0		/* -6 unused. */
	100000000002	/* -5. */
	0		/* -4 unused. */
	040000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADL6:	000000000000	/* 0. */
	0		/* 1 unused. */
	737777777777	/* 2. */
	0		/* 3 unused. */
	677777777776	/* 4. */
	0		/* 5 unused. */
	637777777775	/* 6. */
	0		/* 7 unused. */
	577777777774	/* 8. */
	777777777774	/* 9. */
	0		/* 10 unused. */
	737777777773	/* 11. */
#endif

#ifdef LBADL7
	GLOBAL(%BADL7)
	140000000004	/* -9. */
	0		/* -8 unused. */
	110000000003	/* -7. */
	0		/* -6 unused. */
	060000000002	/* -5. */
	0		/* -4 unused. */
	030000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADL7:	000000000000	/* 0. */
	0		/* 1 unused. */
	747777777777	/* 2. */
	0		/* 3 unused. */
	717777777776	/* 4. */
	0		/* 5 unused. */
	667777777775	/* 6. */
	0		/* 7 unused. */
	637777777774	/* 8. */
#endif

#ifdef LBADL8
	GLOBAL(%BADL8)
	000000000003	/* -7. */
	200000000003	/* -6. */
	000000000002	/* -5. */
	200000000002	/* -4. */
	000000000001	/* -3. */
	200000000001	/* -2. */
	0		/* -1 unused. */
%BADL8:	000000000000	/* 0. */
	577777777777	/* 1. */
	777777777777	/* 2. */
	577777777776	/* 3. */
	777777777776	/* 4. */
	577777777775	/* 5. */
	777777777775	/* 6. */
#endif

#ifdef LBADL9
	GLOBAL(%BADL9)
	140000000003	/* -7. */
	0		/* -6 unused. */
	100000000002	/* -5. */
	0		/* -4 unused. */
	040000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADL9:	000000000000	/* 0. */
	0		/* 1 unused. */
	737777777777	/* 2. */
	0		/* 3 unused. */
	677777777776	/* 4. */
	0		/* 5 unused. */
	637777777775	/* 6. */
#endif

#ifdef LBADLH
	GLOBAL(%BADLH)
	040000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADLH:	000000000000	/* 0. */
	0		/* 1 unused. */
	737777777777	/* 2. */
#endif

#ifdef LBADX6
	GLOBAL(%BADX6)
	777777777773
	017777777773
	0
	777777777774
	017777777774
	0
	777777777775
	017777777775
	0
	777777777776
	017777777776
	0
	777777777777
	017777777777
	0
	0
%BADX6:	000000000000
	0
	760000000001
	000000000001
	0
	760000000002
	000000000002
	0
	760000000003
	000000000003
	0
	760000000004
	000000000004
	0
	760000000005
	000000000005
#endif

#ifdef LBADX7
	GLOBAL(%BADX7)
	777777777774
	007777777774
	0
	0
	0
	777777777775
	007777777775
	0
	0
	0
	777777777776
	007777777776
	0
	0
	0
	777777777777
	007777777777
	0
	0
	0
	0
%BADX7:	000000000000
	0
	0
	0
	770000000001
	000000000001
	0
	0
	0
	770000000002
	000000000002
	0
	0
	0
	770000000003
	000000000003
	0
	0
	0
	770000000004
	000000000004
#endif

#if defined (LBADX8) || defined (LBADX9)
	GLOBAL(%BADX8)
	GLOBAL(%BADX9)
	777777777775
	017777777775
	777777777776
	017777777776
	777777777777
	017777777777
	0
%BADX8:
%BADX9:	000000000000
	760000000001
	000000000001
	760000000002
	000000000002
	760000000003
	000000000003
#endif

#ifdef LBADXH
	GLOBAL(%BADXH)
	777777777777
	007777777777
	0
%BADXH:	0
	770000000001
	000000000001
#endif

	SECTION_END

	FILE_END
