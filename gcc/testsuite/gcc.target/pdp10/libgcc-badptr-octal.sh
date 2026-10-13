#!/bin/sh
# Byte-pointer correction tables are PDP-10 octal words, never decimal data.
set -eu
src=${1:-gcc/config/pdp10/libgcc1.s}
python3 - "$src" <<'PY'
import re
import sys
from pathlib import Path

text = Path(sys.argv[1]).read_text()
start = text.find('/* The following tables are used to calculate byte pointer differences.')
end = text.find('\n\tFILE_END', start)
if start < 0 or end < 0:
    raise SystemExit('byte-pointer table block not found')
for lineno, line in enumerate(text[start:end].splitlines(), 1):
    s = line.strip()
    m = re.match(r'^([0-9]+)(?:\s|$)', s)
    if m and m.group(1) != '0' and not m.group(1).startswith('0'):
        raise SystemExit('non-octal PDP-10 table word: ' + s)
print('PDP-10 libgcc byte-pointer tables use explicit octal words')
PY
