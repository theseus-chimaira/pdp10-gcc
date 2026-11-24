# PDP-10 indexed constant byte-pointer construction v19

Date: 2026-11-24

## Failure and root cause

The comprehensive `pdp10-testkit` byte-pointer difference test
failed in eight GCC O1 configurations at octal case `0250`, the
pointer difference after advancing `char9 *p` from `&arr[3]` by
a dynamic index. A reduced C89 reproducer with a volatile index
failed at O1/O2, but passed at O0.

Generated code used `MOVEI r,POINT 9,arr,35(index)`. MOVEI can
calculate an indexed word address but only writes the 18-bit
effective-address field. It does not preserve the P/S selector of
an actual 36-bit byte pointer. Subsequent IBP and pointer subtraction
therefore used a malformed pointer. Replacing this in a scratch
assembly reproduction with proper byte-pointer construction passed.

## Implementation

The `*addsi3` indexed-symbol alternative is still one MOVEI for
ordinary word addresses. For a symbolic local byte-pointer constant,
`pdp10_output_addsi` decomposes its 36-bit value into a symbol,
18-bit address offset and 18-bit P/S selector. It emits:

```
movei destination,symbol+offset(index)
hrli destination,selector
```

Both instructions are required: MOVEI computes the right-half
effective address (including the index), and HRLI installs the
original left-half selector without changing that address. This
does not introduce a scratch register, works even when destination
and index are the same AC, and avoids a carry from the address into
the byte-pointer selector. The exact alternative reports two words
to GCC's instruction length attribute. Other alternatives continue
reporting their original length. Extended-mode pointers are left
unchanged.

The predicate `pdp10_const_word36_byte_pointer_p` is exported from
the backend so generated attribute code can recognize this exact
case; it uses the same recognition as the instruction emitter.

## Verification boundary

A scratch C89 `char9` pointer difference reproducer now passes all
16 manually tested PDP-6/KA10/KI10/KS10 O0/O1/O2/Os configurations.
The permanent testkit v19 source tests indexed 9-bit and signed
6-bit array pointers through the original compiler driver. Its
independent registered GCC matrix passes **36/36** cells under strict
exit policy.

The broader `runtime-byte-pointer-diff.c` O1 rerun is *not* green.
It now records eight simulator timeouts and one KS10 runtime failure;
the eight former early failures at ID `0250` have advanced past that
assertion, but later pointer handling still needs repair. These
outcomes are not reclassified as successes.

The full compiler matrix and unrelated runtime failures remain open.
