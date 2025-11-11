;
; Bare pdp10-dec-none abort implementation.
;
; The bare target has no process model, stderr, signals, atexit, or core dump
; convention yet.  abort therefore means a deterministic machine stop and no
; return.  HALT may be continued by an operator or simulator, so immediately
; branch back and halt again.
;

	TITLE abort
	ENTRY abort
abort:
	HALT
	JRST abort
	END
