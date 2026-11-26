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

#ifdef L_cmpdf2
; Three-way comparison of canonical D-format doubles.  Compare the signed
; high words first.  If they are equal, AC2 and AC4 have the same copied
; sign bit, so ordinary signed comparison gives the correct continuation
; ordering as well.
	GLOBAL(FUNC(cmpdf2))
FUNC(cmpdf2):
	CAMN 1,3
	 JRST %Lcmpdf_low
	CAMGE 1,3
	 JRST %Lcmpdf_less
	JRST %Lcmpdf_greater
%Lcmpdf_low:
	CAMN 2,4
	 JRST %Lcmpdf_equal
	CAMGE 2,4
	 JRST %Lcmpdf_less
%Lcmpdf_greater:
	MOVEI 1,1
	RETURN
%Lcmpdf_less:
	SETO 1,
	RETURN
%Lcmpdf_equal:
	SETZ 1,
	RETURN
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
; Convert canonical PDP-10 single floating to signed 36-bit SImode.
; Previous HLRE/TRC/ASH simulation lost the signed minimum (-2^35),
; returning zero when the normalized mantissa was shifted into bit35.
; Recover the positive magnitude with a logical shift and then apply
; the original sign; -2^35 naturally remains unchanged by MOVNS.
; AC1 is argument/result; AC5/AC6 are call-clobbered scratch.
	GLOBAL(FUNC(fixsfsi))
FUNC(fixsfsi):
	MOVE 5,1
	JUMPE 1,%Lfixsf_done
	JUMPGE 1,%Lfixsf_pos
	MOVNS 1
%Lfixsf_pos:
	MOVE 6,1
	LSH 6,-033
	ANDI 6,0377
	SUBI 6,0201
	JUMPL 6,%Lfixsf_zero
	AND 1,[0777777777]
	SUBI 6,032
	LSH 1,0(6)
	JUMPGE 5,%Lfixsf_done
	MOVNS 1
%Lfixsf_done:
	RETURN
%Lfixsf_zero:
	SETZ 1,
	RETURN
#endif

#ifdef L_fixunssfsi
; Convert a nonnegative normalized PDP-10 single float to unsigned
; 36-bit SImode, truncating toward zero.  No signed conversion helper
; is suitable above 2^35-1.  For all defined C conversions, the
; incoming value lies in the representable 0..2^36-1 range.
;
; Positive SF: biased exponent in bits 27..34, 27-bit normalized
; fraction in bits 0..26.  The leading fraction bit represents
; 2^(exponent-0201).  Shift by exponent-0201-26 to recover the
; unsigned integer.  AC6 scratch; AC1 input/result.
	GLOBAL(FUNC(fixunssfsi))
FUNC(fixunssfsi):
	JUMPE 1,%Lfixunsf_done
	JUMPL 1,%Lfixunsf_zero
	MOVE 6,1
	LSH 6,-033
	ANDI 6,0377
	SUBI 6,0201
	JUMPL 6,%Lfixunsf_zero
	AND 1,[0777777777]
	SUBI 6,032
	LSH 1,0(6)
%Lfixunsf_done:
	RETURN
%Lfixunsf_zero:
	SETZ 1,
	RETURN
#endif

#ifdef L_fixunssfdi
; PDP-6/KA10 canonical SF -> unsigned 71-bit DImode, truncate toward
; zero.  Unlike the generic libgcc2 implementation, this does not
; require soft D-format division, subtraction or any hosted libc.
;
; Input:  AC1 = canonical PDP-10 single-precision float.
; Output: AC1:AC2 = unsigned 71-bit integer (36-bit high word,
;         35-bit low payload, bit35 of low word copies high sign).
;
; For nonnegative SF, integer value is 27-bit fraction F times
; 2^(unbiased_exponent-26).  Put F in low AC2 of a 72-bit pair,
; shift the pair by unbiased_exponent-25 (one extra bit), and then
; logically shift AC2 back by one.  That positions bit35 exactly as
; the high/low divider boundary without discarding significant bits.
; The final low sign-copy bit follows AC1 bit35.  Values below 1
; truncate to zero.  Negative-to-unsigned casts are outside the
; defined C range; return zero deterministically for those too.
;
; Only AC6 is scratch, preserved across the libcall.  No heap, static
; tables, or calls to other conversion helpers are required.
	GLOBAL(FUNC(fixunssfdi))
FUNC(fixunssfdi):
	JUMPE 1,%Lfixunsfdi_zero
	JUMPL 1,%Lfixunsfdi_zero
	PUSH 17,6
	MOVE 6,1
	LSH 6,-033
	ANDI 6,0377
	SUBI 6,0201
	JUMPL 6,%Lfixunsfdi_small
	SUBI 6,031
	MOVE 2,1
	AND 2,[0777777777]
	SETZ 1,
	LSHC 1,0(6)
	LSH 2,-1
	TLZ 2,0400000
	TLNE 1,0400000
	 TLO 2,0400000
	POP 17,6
	RETURN
%Lfixunsfdi_small:
	POP 17,6
%Lfixunsfdi_zero:
	SETZ 1,
	SETZ 2,
	RETURN
#endif

#ifdef L_fixunsdfdi
; PDP-6/KA10 canonical D-format double -> unsigned 71-bit integer.
; Convert a positive D-format fraction directly using integer shifts,
; not the generic hosted libgcc2 floating divide/subtract sequence.
;
; AC1:AC2 input is fraction27 high plus the 35-bit continuation.
; Reconstruct a 72-bit normalized pair from the fraction and shift
; right by (71 - actual exponent).  AC1:AC2 output is high36:low35
; with a copy of AC1's sign bit into AC2 bit35.  Truncates toward zero.
; Values below 1 return zero; negative unsigned conversions are outside
; defined C range and return zero deterministically.  Save AC3..AC6.
	GLOBAL(FUNC(fixunsdfdi))
FUNC(fixunsdfdi):
	JUMPE 1,%Lfixundfdi_zero
	JUMPL 1,%Lfixundfdi_zero
	PUSH 17,3
	PUSH 17,4
	PUSH 17,5
	PUSH 17,6
	MOVE 3,1
	LSH 3,-033
	ANDI 3,0377
	SUBI 3,0200
	JUMPLE 3,%Lfixundfdi_small
; Align the 62-bit D-format fraction at the top of 72 bits.
	MOVE 4,1
	AND 4,[0777777777]
	LSH 4,011
	MOVE 5,2
	LSH 5,-032
	IOR 4,5
	MOVE 5,2
	LSH 5,012
	MOVE 1,4
	MOVE 2,5
	MOVE 6,3
	SUBI 6,0107
	LSHC 1,0(6)
	LSH 2,-1
	TLZ 2,0400000
	TLNE 1,0400000
	 TLO 2,0400000
	JRST %Lfixundfdi_done
%Lfixundfdi_small:
	SETZ 1,
	SETZ 2,
%Lfixundfdi_done:
	POP 17,6
	POP 17,5
	POP 17,4
	POP 17,3
	RETURN
%Lfixundfdi_zero:
	SETZ 1,
	SETZ 2,
	RETURN
#endif

#ifdef L_fixdfdi
; PDP-6/KA10 canonical D-format double -> signed 71-bit integer.
; Preserve the original sign, form the absolute positive D-format
; 71-bit two's complement, use the exact unsigned integer converter,
; then apply the sign across the 36+35-bit fixed result.  Preserve
; zero as canonical all-zero words even for negative fractions < 1.
	GLOBAL(FUNC(fixdfdi))
FUNC(fixdfdi):
	PUSH 17,5
	MOVE 5,1
	JUMPGE 5,%Lfixdfdi_pos
	SETCA 1,1
	SETCA 2,2
	AND 2,[0377777777777]
	ADDI 2,1
	TLNE 2,0400000
	 ADDI 1,1
	AND 2,[0377777777777]
%Lfixdfdi_pos:
	PUSHJ 17,FUNC(fixunsdfdi)
	JUMPGE 5,%Lfixdfdi_done
	MOVE 6,1
	IOR 6,2
	JUMPE 6,%Lfixdfdi_done
	SETCA 1,1
	SETCA 2,2
	AND 2,[0377777777777]
	ADDI 2,1
	TLNE 2,0400000
	 ADDI 1,1
	AND 2,[0377777777777]
	TLO 2,0400000
%Lfixdfdi_done:
	POP 17,5
	RETURN
#endif

#ifdef L_fixsfdi
; PDP-6/KA10 canonical SF -> signed 71-bit DImode, truncate toward
; zero.  Use the exact full-width unsigned magnitude converter and
; negate the 36+35-bit integer result as one 71-bit two's complement.
; The low word's bit35 is a copy of the signed high word's bit35.
;
; AC1 holds the input single float.  AC1/AC2 return the signed pair;
; AC5 is saved to preserve the pre-conversion floating sign across
; the call, which may use the 71-bit carry boundary.
	GLOBAL(FUNC(fixsfdi))
FUNC(fixsfdi):
	PUSH 17,5
	MOVE 5,1
	JUMPGE 5,%Lfixsfdi_positive
	MOVNS 1
%Lfixsfdi_positive:
	PUSHJ 17,FUNC(fixunssfdi)
	JUMPGE 5,%Lfixsfdi_done
	; A fractional negative value between -1 and 0 truncates to
	; integer zero.  Do not manufacture a signed-71 negative zero
	; by setting the low-word sign-copy bit on an empty magnitude.
	MOVE 6,1
	IOR 6,2
	JUMPE 6,%Lfixsfdi_done
	SETCA 1,1
	SETCA 2,2
	AND 2,[0377777777777]
	ADDI 2,1
	TLNE 2,0400000
	 ADDI 1,1
	AND 2,[0377777777777]
	TLO 2,0400000
%Lfixsfdi_done:
	POP 17,5
	RETURN
#endif

#ifdef L_fixdfsi
; Convert canonical PDP-10 D-format double in AC1/AC2 to signed SImode.
; D-format is a 71-bit two's-complement value: AC2 bit 35 copies the
; sign and the remaining 35 bits continue the fraction.
	GLOBAL(FUNC(fixdfsi))
FUNC(fixdfsi):
	MOVE 5,1
	JUMPGE 5,%Lfixdf_mag
; Form the positive 71-bit magnitude without using DMOVN, which is absent
; on the PDP-6/KA10 baseline.
	SETCA 1,1
	SETCA 2,2
	TLZ 2,0400000
	ADDI 2,1
	TLNN 2,0400000
	 JRST %Lfixdf_nocarry
	TLZ 2,0400000
	AOJA 1,%Lfixdf_mag
%Lfixdf_nocarry:
	TLZ 2,0400000
%Lfixdf_mag:
; Extract k = exponent - 0201.  Values smaller than one truncate to zero.
	MOVE 6,1
	LSH 6,-033
	ANDI 6,0377
	SUBI 6,0201
	JUMPGE 6,%Lfixdf_nonfrac
	SETZ 1,
	RETURN
%Lfixdf_nonfrac:
	CAIG 6,032
	 JRST %Lfixdf_highonly
; k > 26: combine the 27 high fraction bits with up to eight continuation
; bits from AC2.
	MOVE 7,6
	SUBI 7,032
	AND 1,[0777777777]
	LSH 1,0(7)
	MOVEI 4,043
	SUB 4,7
	MOVN 4,4
	MOVE 3,2
	LSH 3,0(4)
	IOR 1,3
	JRST %Lfixdf_sign
%Lfixdf_highonly:
	AND 1,[0777777777]
	MOVEI 7,032
	SUB 7,6
	MOVN 7,7
	LSH 1,0(7)
%Lfixdf_sign:
	JUMPGE 5,%Lfixdf_done
	MOVNS 1
%Lfixdf_done:
	RETURN
#endif

#ifdef L_floatsisf
; Convert signed 36-bit SImode to a NORMALIZED PDP-10 SF-format value.
; An FSC on an unnormalized integer sets an exponent, but leaves its
; mantissa at its original low bit position.  Thus FSC 1,0233 on 1024
; produces 233000002000, not the canonical 213400000000 value.
;
; The result is exponent (0201 + floor(log2(abs(n)))) and a 27-bit
; fraction aligned at bit 26.  Rounding to nearest/ties-to-even when
; the integer has more than 27 significant bits compares the lost low
; bits to the half-ULP without modifying the full magnitude.  A
; fraction carry increments the exponent.  Negative results
; are the two's complement of the entire positive floating word.
; 0 and signed -2^35 have explicitly defined canonical representations.
; AC5 is call-clobbered.  Preserve AC3/AC4/AC6/AC7 around the
; normalization loop; GCC may hold caller values in those registers
; across a libcall.  No static data or heap is needed.
	GLOBAL(FUNC(floatsisf))
FUNC(floatsisf):
	MOVE 5,1
	JUMPN 1,%Lflts_nonzero
	RETURN
%Lflts_nonzero:
	CAMN 1,[0400000000000]
	 JRST %Lflts_min
	PUSH 17,3
	PUSH 17,4
	PUSH 17,6
	PUSH 17,7
	JUMPGE 5,%Lflts_absdone
	MOVNS 1

%Lflts_absdone:
; Find k, highest set bit of the positive magnitude (0..34).
	MOVE 6,1
	MOVEI 7,042
%Lflts_scan:
	TLNE 6,0200000
	 JRST %Lflts_found
	LSH 6,1
	SOJA 7,%Lflts_scan
%Lflts_found:
	MOVEI 6,032
	SUB 6,7
	JUMPGE 6,%Lflts_shift
; More than 27 integer bits: compare discarded remainder to half,
; and when equal round only if the retained fraction's LSB is 1.
	MOVE 3,1
	MOVN 4,6
	MOVEI 6,1
	LSH 6,0(4)
	MOVE 4,6
	SUBI 4,1
	AND 3,4
	MOVEI 6,032
	SUB 6,7
	LSH 1,0(6)
	MOVE 4,7
	SUBI 4,033
	MOVEI 6,1
	LSH 6,0(4)
	SUB 3,6
	JUMPL 3,%Lflts_exponent
	JUMPG 3,%Lflts_round
	MOVE 3,1
	ANDI 3,1
	JUMPE 3,%Lflts_exponent
%Lflts_round:
	AOJ 1,
	CAME 1,[01000000000]
	 JRST %Lflts_exponent
	LSH 1,-1
	AOJ 7,
	JRST %Lflts_exponent
%Lflts_shift:
	LSH 1,0(6)
%Lflts_exponent:
	MOVEI 6,0201
	ADD 6,7
	LSH 6,033
	IOR 1,6
	JUMPGE 5,%Lflts_done
	MOVNS 1
%Lflts_done:
	POP 17,7
	POP 17,6
	POP 17,4
	POP 17,3
	RETURN
%Lflts_min:
	MOVE 1,[0533400000000]
	RETURN
#endif

#ifdef L_floatunssisf
; Convert the full unsigned 36-bit SImode range into canonical PDP-10 SF.
; The old FSC-only conversion retained the integer in the low mantissa
; bits and discarded nine bits of high unsigned input before rounding.
; Instead find the highest set bit, align a 27-bit fraction, and round
; nearest with ties to even.  A carry out of the fraction increments
; the exponent, so 2^36-1 correctly rounds to exactly 2^36.
; Preserve AC3/AC4/AC6/AC7, which GCC may keep live over a libcall;
; no persistent storage or helper dependency is needed.
	GLOBAL(FUNC(floatunssisf))
FUNC(floatunssisf):
	JUMPN 1,%Luflts_nonzero
	RETURN
%Luflts_nonzero:
	PUSH 17,3
	PUSH 17,4
	PUSH 17,6
	PUSH 17,7
	MOVE 6,1
	MOVEI 7,043
%Luflts_scan:
	TLNE 6,0400000
	 JRST %Luflts_found
	LSH 6,1
	SOJA 7,%Luflts_scan
%Luflts_found:
	MOVEI 6,032
	SUB 6,7
	JUMPGE 6,%Luflts_shift
; Right-shift by k-26.  Preserve discarded bits for exact rounding;
; never add to the full 36-bit source, which could overflow.
	MOVE 3,1
	MOVN 4,6
	MOVEI 6,1
	LSH 6,0(4)
	MOVE 4,6
	SUBI 4,1
	AND 3,4
	MOVEI 6,032
	SUB 6,7
	LSH 1,0(6)
	MOVE 4,7
	SUBI 4,033
	MOVEI 6,1
	LSH 6,0(4)
	SUB 3,6
	JUMPL 3,%Luflts_exponent
	JUMPG 3,%Luflts_round
	MOVE 3,1
	ANDI 3,1
	JUMPE 3,%Luflts_exponent
%Luflts_round:
	AOJ 1,
	CAME 1,[01000000000]
	 JRST %Luflts_exponent
	LSH 1,-1
	AOJ 7,
	JRST %Luflts_exponent
%Luflts_shift:
	LSH 1,0(6)
%Luflts_exponent:
	MOVEI 6,0201
	ADD 6,7
	LSH 6,033
	IOR 1,6
	POP 17,7
	POP 17,6
	POP 17,4
	POP 17,3
	RETURN
#endif

#ifdef L_floatsidf
; Convert signed SImode in AC1 to canonical PDP-10 D-format double in
; AC1/AC2.  Build the 27+35 fraction explicitly so no floating instruction
; newer than the PDP-6 baseline is required.
	GLOBAL(FUNC(floatsidf))
FUNC(floatsidf):
	JUMPN 1,%Lfltd_nonzero
	SETZ 2,
	RETURN
%Lfltd_nonzero:
	MOVE 4,1
; -2^35 has no positive SImode magnitude; its D-format representation is
; exact and unique, so handle it directly.
	CAMN 1,[0400000000000]
	 JRST %Lfltd_min
	JUMPGE 1,%Lfltd_absdone
	MOVNS 1
%Lfltd_absdone:
; Find k=floor(log2(magnitude)).  Code size and RAM are more important than
; a large leading-zero table; at most 34 iterations are required.
	MOVE 5,1
	MOVEI 6,042
%Lfltd_scan:
	TLNE 5,0200000
	 JRST %Lfltd_found
	LSH 5,1
	SOJA 6,%Lfltd_scan
%Lfltd_found:
	CAIG 6,032
	 JRST %Lfltd_small
; k > 26.  Keep the top 27 fraction bits in AC1 and place the remaining
; low bits at the top of AC2's 35-bit continuation field.
	MOVE 7,6
	SUBI 7,032
	MOVE 3,1
	MOVN 5,7
	LSH 1,0(5)
	MOVEI 5,043
	SUB 5,7
	LSH 3,0(5)
	TLZ 3,0400000
	MOVE 2,3
	JRST %Lfltd_exponent
%Lfltd_small:
	MOVEI 5,032
	SUB 5,6
	LSH 1,0(5)
	SETZ 2,
%Lfltd_exponent:
	MOVEI 5,0201
	ADD 5,6
	LSH 5,033
	IOR 1,5
	JUMPGE 4,%Lfltd_done
; Negate the 71-bit D-format fraction and copy the resulting high sign into
; AC2 bit 35.
	SETCA 1,1
	SETCA 2,2
	TLZ 2,0400000
	ADDI 2,1
	TLNN 2,0400000
	 JRST %Lfltd_neg_nocarry
	TLZ 2,0400000
	AOJA 1,%Lfltd_neg_sign
%Lfltd_neg_nocarry:
	TLZ 2,0400000
%Lfltd_neg_sign:
	TLO 2,0400000
%Lfltd_done:
	RETURN
%Lfltd_min:
	MOVE 1,[0533400000000]
	MOVE 2,[0400000000000]
	RETURN
#endif

#ifdef L_floatunsidf
; Exact 36-bit unsigned SImode -> canonical PDP-10 D-format double.
; A double has a 62-bit fraction, so all 36 unsigned input bits fit
; without rounding.  Converting through SF loses low precision bits
; for values >= 2^27 and is not a valid replacement for this helper.
; Input AC1 is unsigned; output AC1/AC2 contains the full D-format.
; The fraction is 27 high bits plus 35 low continuation bits.
	GLOBAL(FUNC(floatunsidf))
FUNC(floatunsidf):
	JUMPN 1,%Lufltd_nonzero
	SETZ 2,
	RETURN
%Lufltd_nonzero:
	PUSH 17,3
	PUSH 17,4
	PUSH 17,5
	PUSH 17,6
	PUSH 17,7
; Find the unsigned highest set bit k (0..35), including bit 35.
	MOVE 5,1
	MOVEI 6,043
%Lufltd_scan:
	TLNE 5,0400000
	 JRST %Lufltd_found
	LSH 5,1
	SOJA 6,%Lufltd_scan
%Lufltd_found:
	CAIG 6,032
	 JRST %Lufltd_small
; k>26: retain high 27 bits in AC1, put the remaining low bits
; in AC2 shifted to the high end of its 35-bit continuation.
	MOVE 7,6
	SUBI 7,032
	MOVE 3,1
	MOVN 4,7
	LSH 1,0(4)
	MOVEI 4,043
	SUB 4,7
	LSH 3,0(4)
	TLZ 3,0400000
	MOVE 2,3
	JRST %Lufltd_exponent
%Lufltd_small:
	MOVEI 4,032
	SUB 4,6
	LSH 1,0(4)
	SETZ 2,
%Lufltd_exponent:
	MOVEI 4,0201
	ADD 4,6
	LSH 4,033
	IOR 1,4
	POP 17,7
	POP 17,6
	POP 17,5
	POP 17,4
	POP 17,3
	RETURN
#endif

/* DAIMOS/PDP-10 freestanding libgcc supplement.

   Keep each independently useful helper in a distinct LIB1ASMFUNCS object.
   This lets the archive linker pull only helpers actually referenced by a
   program.  The four DImode division/modulo entry points intentionally remain
   together because they share the same substantial private 71-bit divider.  */

#ifdef L_floatdisf
; Signed 71-bit DImode -> canonical PDP-10 SF, round nearest/ties-even.
;
; Input AC1:AC2 is GCC's signed high36:low35 integer with a sign-copy
; bit at AC2 bit35.  Normalize all 71 integer bits first, then choose
; the top 27 bits as the SF fraction.  The guard bit is normalized
; high-word bit8; the sticky bits are high-word bits0..7 and ALL of
; normalized AC2.  A D-format intermediate would lose some of those
; bits and can introduce double rounding; never use it here.
;
; Return canonical SF in AC1.  This is PDP-6/KA10 base ISA code, with
; no heap, permanent table, or OS call.  Preserve AC3..AC6; AC7 is volatile.
; Unsigned entry shares the magnitude conversion, not the signed
; interpretation of the high limb. AC7 carries the signed-mode flag
; until magnitude normalization, and then the negative-result flag.
	GLOBAL(FUNC(floatundisf))
FUNC(floatundisf):
	SETZ 7,
	JRST %Lfdsf_entry
	GLOBAL(FUNC(floatdisf))
FUNC(floatdisf):
	MOVEI 7,1
%Lfdsf_entry:
	PUSH 17,3
	PUSH 17,4
	PUSH 17,5
	PUSH 17,6
	MOVE 3,1
	IOR 3,2
	JUMPE 3,%Lfdsf_zero
	JUMPE 7,%Lfdsf_unsigned
	JUMPL 1,%Lfdsf_abs
	SETZ 7,
	JRST %Lfdsf_unsigned
%Lfdsf_abs:
	SETCA 1,1
	SETCA 2,2
	AND 2,[0377777777777]
	ADDI 2,1
	TLNE 2,0400000
	 ADDI 1,1
	AND 2,[0377777777777]
%Lfdsf_unsigned:
	LSH 2,1
	MOVEI 3,0107
%Lfdsf_normalize:
	TLNE 1,0400000
	 JRST %Lfdsf_mantissa
	LSHC 1,1
	SOJA 3,%Lfdsf_normalize
%Lfdsf_mantissa:
; 72-bit normalized integer: AC1 high bit35 is the implicit 1.
	MOVE 4,1
	LSH 4,-011
	MOVE 5,1
	ANDI 5,0777
	SUBI 5,0400
	JUMPL 5,%Lfdsf_pack
	JUMPG 5,%Lfdsf_round
; Exactly half an SF ULP at bit8: any remaining low bit means > half.
	JUMPN 2,%Lfdsf_round
	TRNE 4,1
	 JRST %Lfdsf_round
	JRST %Lfdsf_pack
%Lfdsf_round:
	AOJ 4,
	CAME 4,[01000000000]
	 JRST %Lfdsf_pack
	LSH 4,-1
	AOJ 3,
%Lfdsf_pack:
	ADDI 3,0200
	LSH 3,033
	IOR 4,3
	MOVE 1,4
	JUMPE 7,%Lfdsf_done
	MOVNS 1
	JRST %Lfdsf_done
%Lfdsf_zero:
	SETZ 1,
%Lfdsf_done:
	POP 17,6
	POP 17,5
	POP 17,4
	POP 17,3
	RETURN
#endif

#ifdef L_floatdidf
; Signed 71-bit DImode -> PDP-10 two-word D-format, exactly preserving
; the 71-bit ABI sign and the low 35-bit continuation.  This is the
; corresponding compiler-owned GCC entry to the previously verified
; PDP-6 KCC integer-only normalized conversion algorithm.
;
; Input AC1 high36, AC2 low35 plus sign copy.  Output AC1/AC2 is
; canonical 27+35-bit D-format, rounded nearest/ties-to-even.  The
; generic libgcc2 arithmetic used a host-style 72-bit word split and
; produced wrong D-format values at 2^35-1 and at larger boundaries.
; Only PDP-6 instructions are used; preserve AC3..AC6; AC7 is volatile.
; Unsigned entry shares the magnitude conversion, not the signed
; interpretation of the high limb. AC7 carries the signed-mode flag
; until magnitude normalization, and then the negative-result flag.
	GLOBAL(FUNC(floatundidf))
FUNC(floatundidf):
	SETZ 7,
	JRST %Lfd71_entry
	GLOBAL(FUNC(floatdidf))
FUNC(floatdidf):
	MOVEI 7,1
%Lfd71_entry:
	PUSH 17,3
	PUSH 17,4
	PUSH 17,5
	PUSH 17,6
	MOVE 3,1
	IOR 3,2
	JUMPE 3,%Lfd71_zero
	JUMPE 7,%Lfd71_unsigned
	JUMPL 1,%Lfd71_abs
	SETZ 7,
	JRST %Lfd71_unsigned
%Lfd71_abs:
	SETCA 1,1
	SETCA 2,2
	AND 2,[0377777777777]
	ADDI 2,1
	TLNE 2,0400000
	 ADDI 1,1
	AND 2,[0377777777777]
%Lfd71_unsigned:
	LSH 2,1
	MOVEI 3,0107
%Lfd71_normalize:
	TLNE 1,0400000
	 JRST %Lfd71_mantissa
	LSHC 1,1
	SOJA 3,%Lfd71_normalize
%Lfd71_mantissa:
	MOVE 4,1
	LSH 4,-011
	MOVE 5,1
	ANDI 5,0777
	LSH 5,032
	MOVE 6,2
	LSH 6,-012
	IOR 5,6
; Ten lost low bits: guard bit9 and sticky bits8..0.
	MOVE 6,2
	ANDI 6,01777
	SUBI 6,01000
	JUMPL 6,%Lfd71_pack
	JUMPE 6,%Lfd71_tie
	JRST %Lfd71_round
%Lfd71_tie:
	TRNE 5,1
	 JRST %Lfd71_round
	JRST %Lfd71_pack
%Lfd71_round:
	ADDI 5,1
	TLNE 5,0400000
	 JRST %Lfd71_carry
	JRST %Lfd71_pack
%Lfd71_carry:
	AND 5,[0377777777777]
	ADDI 4,1
	MOVE 6,4
	LSH 6,-033
	JUMPE 6,%Lfd71_pack
	MOVE 4,[0400000000]
	AOS 3
%Lfd71_pack:
	ADDI 3,0200
	LSH 3,033
	IOR 4,3
	MOVE 1,4
	MOVE 2,5
	JUMPE 7,%Lfd71_done
; Two's-complement the whole D-format 71-bit pair, not each word.
	SETCA 1,1
	SETCA 2,2
	AND 2,[0377777777777]
	ADDI 2,1
	TLNE 2,0400000
	 ADDI 1,1
	AND 2,[0377777777777]
	TLO 2,0400000
	JRST %Lfd71_done
%Lfd71_zero:
	SETZ 1,
	SETZ 2,
%Lfd71_done:
	POP 17,6
	POP 17,5
	POP 17,4
	POP 17,3
	RETURN
#endif

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
; PDP-10 AC numbers in this assembly are octal. The unsigned core
; reuses the shifting numerator pair for the quotient, 5:6 for the
; remainder or hardware DIV pair, 7 for the count, and 10 for scratch.
; AC5..AC10 (octal) are saved; signed wrappers also save AC13 for sign.
; All paths use PDP-6/KA10 base instructions and no extended addressing.
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
	push 017,5
	push 017,6
	push 017,7
	push 017,010
	pushj 017,__p010_udivmoddi4
	pop 017,010
	pop 017,7
	pop 017,6
	pop 017,5
	popj 017,

__umoddi3:
	push 017,5
	push 017,6
	push 017,7
	push 017,010
	pushj 017,__p010_udivmoddi4
	move 1,3
	move 2,4
	pop 017,010
	pop 017,7
	pop 017,6
	pop 017,5
	popj 017,

__divdi3:
	push 017,5
	push 017,6
	push 017,7
	push 017,010
	push 017,013
	setz 013,
	and 2,[0377777777777]
	and 4,[0377777777777]
	tlne 1,0400000
	 pushj 017,__p010_abs_num_q
	tlne 3,0400000
	 pushj 017,__p010_abs_den_q
	pushj 017,__p010_udivmoddi4
	jumpe 013,__p010_div_signcopy
	pushj 017,__p010_neg_012
__p010_div_signcopy:
	pushj 017,__p010_signcopy_012
	pop 017,013
	pop 017,010
	pop 017,7
	pop 017,6
	pop 017,5
	popj 017,

__moddi3:
	push 017,5
	push 017,6
	push 017,7
	push 017,010
	push 017,013
	setz 013,
	and 2,[0377777777777]
	and 4,[0377777777777]
	tlne 1,0400000
	 pushj 017,__p010_abs_num_r
	tlne 3,0400000
	 pushj 017,__p010_abs_den_r
	pushj 017,__p010_udivmoddi4
	move 1,3
	move 2,4
	jumpe 013,__p010_mod_signcopy
	pushj 017,__p010_neg_012
__p010_mod_signcopy:
	pushj 017,__p010_signcopy_012
	pop 017,013
	pop 017,010
	pop 017,7
	pop 017,6
	pop 017,5
	popj 017,

; Negate AC1:AC2 as one PDP-010 DImode value: 36 high bits + 35 low bits.
__p010_neg_012:
	movn 1,1
	skipe 2
	 subi 1,1
	movn 2,2
	and 2,[0377777777777]
	popj 017,

; Negate AC3:AC4 as one PDP-010 DImode value: 36 high bits + 35 low bits.
__p010_neg_34:
	movn 3,3
	skipe 4
	 subi 3,1
	movn 4,4
	and 4,[0377777777777]
	popj 017,

; Restore GCC's external signed-DImode representation in AC1:AC2.
; Bit 35 of the low word duplicates the sign of the high word.
__p010_signcopy_012:
	tlz 2,0400000
	tlne 1,0400000
	 tlo 2,0400000
	popj 017,

__p010_abs_num_q:
	movei 013,1
	pushj 017,__p010_neg_012
	popj 017,

__p010_abs_den_q:
	xori 013,1
	pushj 017,__p010_neg_34
	popj 017,

__p010_abs_num_r:
	movei 013,1
	pushj 017,__p010_neg_012
	popj 017,

__p010_abs_den_r:
	pushj 017,__p010_neg_34
	popj 017,

; Unsigned divide AC1:AC2 by AC3:AC4 as 71-bit values.
; Each pair is 36 high bits + 35 low bits.  Returns quotient in AC1:AC2
; and remainder in AC3:AC4, both with low bit 35 clear.
__p010_udivmoddi4:
	tlz 2,0400000
	tlz 4,0400000
	jumpn 3,__p010_udivmod_wide
	jumpn 4,__p010_udivmod_narrow
	setz 1,
	setz 2,
	popj 017,

; A low35 divisor is positive. For high36 = 0, IDIV handles
; the entire numerator. For divisor 1, return the original pair.
; Otherwise divide the unsigned high word (split into a positive
; 70-bit pair), then divide remainder:low35. Both quotients fit
; in a positive signed word; the latter is strictly below 2^35.
__p010_udivmod_narrow:
	caie 4,1
	 jrst __p010_udivmod_narrow_gt1
	setz 4,
	popj 017,
__p010_udivmod_narrow_gt1:
	jumpn 1,__p010_udivmod_two_div
	idiv 2,4
	move 4,3
	setz 3,
	popj 017,
__p010_udivmod_two_div:
	move 5,1
	lsh 5,-043
	move 6,1
	tlz 6,0400000
	div 5,4
	move 1,5
	move 5,6
	move 6,2
	div 5,4
	move 2,5
	move 4,6
	popj 017,

; General restoring divider: numerator bits shift out at the top,
; quotient bits enter at the bottom. Bias the dying divisor high
; word once for unsigned comparisons; subtracting it and toggling
; the high sign bit is subtraction modulo 2^36.
__p010_udivmod_wide:
	tlc 3,0400000
	setz 5,
	setz 6,
	movei 7,0107
__p010_udivmod_loop:
	move 010,1
	lsh 010,-043
	lsh 1,1
	lsh 2,1
	skipge 2
	 iori 1,1
	tlz 2,0400000
	lsh 5,1
	lsh 6,1
	skipge 6
	 iori 5,1
	tlz 6,0400000
	ior 6,010
	move 010,5
	tlc 010,0400000
	caml 010,3
	 jrst __p010_udivmod_hi_ge
	jrst __p010_udivmod_no_sub
__p010_udivmod_hi_ge:
	came 010,3
	 jrst __p010_udivmod_sub
	caml 6,4
	 jrst __p010_udivmod_sub
	jrst __p010_udivmod_no_sub
__p010_udivmod_sub:
	sub 6,4
	jumpge 6,__p010_udivmod_no_borrow
	tlc 6,0400000
	subi 5,1
__p010_udivmod_no_borrow:
	sub 5,3
	tlc 5,0400000
	iori 2,1
__p010_udivmod_no_sub:
	sojg 7,__p010_udivmod_loop
	move 3,5
	move 4,6
	popj 017,

#endif


	SECTION_END

	SECTION_RO (.rodata)

/* The following tables are used to calculate byte pointer differences.
   They are taken, with small changes, from KCC.

   Keep every nonzero table word explicitly octal.  DAS follows GAS/C literal
   syntax for DOBJ input, so a word such as 140000000003 without a leading
   zero is decimal and silently assembles to the wrong 36-bit value.  */

#ifdef LBADL6
	GLOBAL(%BADL6)
	040000000005	/* -12. */
	0		/* -11 unused. */
	000000000004	/* -10. */
	0200000000004	/* -9. */
	0		/* -8 unused. */
	0140000000003	/* -7. */
	0		/* -6 unused. */
	0100000000002	/* -5. */
	0		/* -4 unused. */
	040000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADL6:	000000000000	/* 0. */
	0		/* 1 unused. */
	0737777777777	/* 2. */
	0		/* 3 unused. */
	0677777777776	/* 4. */
	0		/* 5 unused. */
	0637777777775	/* 6. */
	0		/* 7 unused. */
	0577777777774	/* 8. */
	0777777777774	/* 9. */
	0		/* 10 unused. */
	0737777777773	/* 11. */
#endif

#ifdef LBADL7
	GLOBAL(%BADL7)
	0140000000004	/* -9. */
	0		/* -8 unused. */
	0110000000003	/* -7. */
	0		/* -6 unused. */
	060000000002	/* -5. */
	0		/* -4 unused. */
	030000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADL7:	000000000000	/* 0. */
	0		/* 1 unused. */
	0747777777777	/* 2. */
	0		/* 3 unused. */
	0717777777776	/* 4. */
	0		/* 5 unused. */
	0667777777775	/* 6. */
	0		/* 7 unused. */
	0637777777774	/* 8. */
#endif

#ifdef LBADL8
	GLOBAL(%BADL8)
	000000000003	/* -7. */
	0200000000003	/* -6. */
	000000000002	/* -5. */
	0200000000002	/* -4. */
	000000000001	/* -3. */
	0200000000001	/* -2. */
	0		/* -1 unused. */
%BADL8:	000000000000	/* 0. */
	0577777777777	/* 1. */
	0777777777777	/* 2. */
	0577777777776	/* 3. */
	0777777777776	/* 4. */
	0577777777775	/* 5. */
	0777777777775	/* 6. */
#endif

#ifdef LBADL9
	GLOBAL(%BADL9)
	0140000000003	/* -7. */
	0		/* -6 unused. */
	0100000000002	/* -5. */
	0		/* -4 unused. */
	040000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADL9:	000000000000	/* 0. */
	0		/* 1 unused. */
	0737777777777	/* 2. */
	0		/* 3 unused. */
	0677777777776	/* 4. */
	0		/* 5 unused. */
	0637777777775	/* 6. */
#endif

#ifdef LBADLH
	GLOBAL(%BADLH)
	040000000001	/* -3. */
	0		/* -2 unused. */
	0		/* -1 unused. */
%BADLH:	000000000000	/* 0. */
	0		/* 1 unused. */
	0737777777777	/* 2. */
#endif

#ifdef LBADX6
	GLOBAL(%BADX6)
	0777777777773
	017777777773
	0
	0777777777774
	017777777774
	0
	0777777777775
	017777777775
	0
	0777777777776
	017777777776
	0
	0777777777777
	017777777777
	0
	0
%BADX6:	000000000000
	0
	0760000000001
	000000000001
	0
	0760000000002
	000000000002
	0
	0760000000003
	000000000003
	0
	0760000000004
	000000000004
	0
	0760000000005
	000000000005
#endif

#ifdef LBADX7
	GLOBAL(%BADX7)
	0777777777774
	007777777774
	0
	0
	0
	0777777777775
	007777777775
	0
	0
	0
	0777777777776
	007777777776
	0
	0
	0
	0777777777777
	007777777777
	0
	0
	0
	0
%BADX7:	000000000000
	0
	0
	0
	0770000000001
	000000000001
	0
	0
	0
	0770000000002
	000000000002
	0
	0
	0
	0770000000003
	000000000003
	0
	0
	0
	0770000000004
	000000000004
#endif

#if defined (LBADX8) || defined (LBADX9)
	GLOBAL(%BADX8)
	GLOBAL(%BADX9)
	0777777777775
	017777777775
	0777777777776
	017777777776
	0777777777777
	017777777777
	0
%BADX8:
%BADX9:	000000000000
	0760000000001
	000000000001
	0760000000002
	000000000002
	0760000000003
	000000000003
#endif

#ifdef LBADXH
	GLOBAL(%BADXH)
	0777777777777
	007777777777
	0
%BADXH:	0
	0770000000001
	000000000001
#endif

	SECTION_END

	FILE_END
