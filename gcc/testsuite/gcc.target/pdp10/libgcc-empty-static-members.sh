#!/bin/sh
# Verify freestanding PDP-10 static-only libgcc member selection.
set -eu
root=${srcdir-`dirname "$0"`/../../../..}
python3 - "$root/config/pdp10/t-none" "$root/mklibgcc.in" <<'PY'
import re
import sys
from pathlib import Path
text = Path(sys.argv[1]).read_text()
m = re.search(r'^LIB2FUNCS_EXCLUDE\s*=\s*(.*(?:\\\n[ \t]+.*)*)', text, re.M)
if not m:
    raise SystemExit('LIB2FUNCS_EXCLUDE not found')
words = set(m.group(1).replace('\\\n', ' ').split())
for name in ('_eprintf', '_bb'):
    if name not in words:
        raise SystemExit(name + ' is not excluded')
if '__gcc_bcmp' in words:
    raise SystemExit('__gcc_bcmp must remain enabled')
mk = Path(sys.argv[2]).read_text()
if not re.search(r'LIB2FUNCS_ST=`echo \$LIB2FUNCS_ST \| sed', mk):
    raise SystemExit('LIB2FUNCS_ST is not filtered')
PY
echo "pdp10 freestanding static libgcc member selection passed"
