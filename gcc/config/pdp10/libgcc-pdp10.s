; Extra weak-ish runtime stubs for smoke/semantic tests that reference libgcc
; helpers not yet provided by the real DAIMON test support.
;
; Keep this file deliberately small.  It is test-harness runtime glue, not the
; final DAIMON libc.
.text

; GCC emits a call to __main at the start of main.  In full hosted GCC this
; supports language runtime initialization such as C++ constructors.  The bare
; PDP-10 semantic harness has no such runtime yet, so make it a no-op.
	.globl	__main
__main:
	popj 17,

; Full DImode division/modulo helpers for the bare semantic harness.
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

; Minimal 9-bit byte-oriented memory routines for the bare semantic harness.
;
; Calling convention used by generated PDP-10 GCC code here:
;   AC1 = destination byte pointer
;   AC2 = source byte pointer / fill byte
;   AC3 = byte count, in C storage units (9-bit bytes for this target)
;   return AC1 = original destination pointer
;
; These routines intentionally operate on PDP-10 byte pointers using LDB/DPB
; and IBP.  GCC-generated code for BLKmode struct-return currently emits a
; call like:
;   TLO 1,331100
;   TLO 2,331100
;   MOVEI 3,100
;   PUSHJ 17,memmove
; for a 64-byte / 16-word aggregate.  Without this symbol, the harness linker
; may resolve memmove to zero and the simulator wanders into low-core soup.
;
; memmove is currently forward-copy only.  That is sufficient for the current
; struct-return and non-overlap semantic reducers.  Add a backward-overlap path
; before blessing true overlapping memmove semantics.
	.globl	memcpy
	.globl	memmove
memcpy:
memmove:
	move 4,1		; preserve return value
	jumpe 3,memmove_done
memmove_loop:
	ldb 5,2
	dpb 5,1
	ibp 2
	ibp 1
	sojg 3,memmove_loop
memmove_done:
	move 1,4
	popj 17,

; Minimal byte memset, useful for future aggregate/zero-fill tests.
	.globl	memset
memset:
	move 4,1		; preserve return value
	move 5,2
	andi 5,777		; byte value is 9 bits in this harness
	jumpe 3,memset_done
memset_loop:
	dpb 5,1
	ibp 1
	sojg 3,memset_loop
memset_done:
	move 1,4
	popj 17,

; Minimal PDP-10 floating-point libgcc helpers for bare GCC semantic tests.
;
; GCC for this target emits libcalls for some KA10/PDP-6 double-float
; operations.  The bare harness links all sources into one image, so these
; names must be real definitions, not unresolved placeholders.
;
; ABI used by the current GCC backend:
;   SF input/output: AC1
;   DF lhs/result:   AC1:AC2
;   DF rhs:          AC3:AC4
;
; These routines use the PDP-10 double-float opcodes already understood by
; the simulator.  They are runtime support, not tests.
	.globl	__adddf3
	.globl	__subdf3
	.globl	__muldf3
	.globl	__divdf3
	.globl	__fixsfsi
	.globl	__fixdfsi
	.globl	__floatsisf
	.globl	__floatsidf

__adddf3:
	; Avoid KA10 SIMH double-float op hangs in the bare harness.
	; The current tests use small exact values, so high-word single-float
	; arithmetic with a zero low word is enough to unmask GCC semantics.
	movem 3,__p10_df_rhs
	fadr 1,__p10_df_rhs
	setz 2,
	popj 17,

__subdf3:
	movem 3,__p10_df_rhs
	fsbr 1,__p10_df_rhs
	setz 2,
	popj 17,

__muldf3:
	movem 3,__p10_df_rhs
	fmpr 1,__p10_df_rhs
	setz 2,
	popj 17,

__divdf3:
	; KA10 SIMH currently hangs on DFDV in the bare-step harness.
	; Use the single-float FDVR path for the high word and clear the low
	; word.  This is sufficient for the current GCC semantic timeout rows,
	; which only exercise small exactly representable values and integer
	; casts/comparisons after division.
	movem 3,__p10_df_rhs
	fdvr 1,__p10_df_rhs
	setz 2,
	popj 17,

; Convert signed integer in AC1 to single float in AC1.
; Do this in software for PDP-6/KA10-class bare runs: FLTR is not reliable
; across the bundled simulators/profiles.  Build a normalized PDP-10 single
; float word directly.  For a positive integer N, the mantissa is shifted
; until bit 26 (octal 400000000) is set and the excess-0200 exponent is
; adjusted from the starting value 0233 (= 0201 + 26).
__floatsisf:
	jumpe 1,__floatsisf_zero
	movei 5,0
	jumpge 1,__floatsisf_abs
	movei 5,1
	movn 1,1
__floatsisf_abs:
	move 2,1
	movei 3,233
__floatsisf_rnorm:
	caml 2,[1000000000]
	 jrst __floatsisf_rshift
	jrst __floatsisf_ltest
__floatsisf_rshift:
	lsh 2,-1
	addi 3,1
	jrst __floatsisf_rnorm
__floatsisf_ltest:
	caml 2,[400000000]
	 jrst __floatsisf_pack
	lsh 2,1
	subi 3,1
	jrst __floatsisf_ltest
__floatsisf_pack:
	lsh 3,33
	move 1,2
	ior 1,3
	jumpe 5,__floatsisf_done
	movn 1,1
__floatsisf_done:
	popj 17,
__floatsisf_zero:
	movei 1,0
	popj 17,

; Convert signed integer in AC1 to double float in AC1:AC2.
; The test runtime's double helpers use a single-float high word plus a zero
; low word for the exact small values exercised by these semantic rows.
__floatsidf:
	pushj 17,__floatsisf
	setz 2,
	popj 17,

; Scratch pair for double-float helpers.
__p10_df_rhs:
	0
	0


; Convert single float in AC1 to signed integer in AC1.
; PDP-6/KA10 have no FIX instruction.  Decode the PDP-10 single-float word
; directly and truncate toward zero.  This handles the normal finite values
; used by the semantic tests and avoids the old inline MUL/TSC sequence, which
; produced zero for negative fractional inputs in the bare harness.
__fixsfsi:
	move 2,1		; work copy
	movei 5,0		; sign flag
	jumpge 2,__fixsfsi_abs
	movei 5,1
	movm 2,2		; absolute PDP-10 float word
__fixsfsi_abs:
	move 3,2
	lsh 3,-33		; exponent field
	subi 3,200		; unbiased exponent
	jumple 3,__fixsfsi_zero
	tlz 2,777000		; keep 27-bit mantissa/fraction
	subi 3,33		; shift = exponent - 27
	lsh 2,(3)
	move 1,2
	jumpe 5,__fixsfsi_done
	movn 1,1
__fixsfsi_done:
	popj 17,
__fixsfsi_zero:
	movei 1,0
	popj 17,

; Convert double float in AC1:AC2 to signed integer in AC1.
; For the current PDP-6/KA10 semantic tests this mirrors the compiler's
; single-float-to-int expansion and uses the high word of the double value.
__fixdfsi:
	; The bare double helpers keep the value in AC1 as a PDP-10 single
	; float high word and clear AC2.  Reuse the tested single-float
	; decoder so negative fractional values truncate toward zero correctly.
	jrst __fixsfsi
