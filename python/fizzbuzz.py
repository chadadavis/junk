#!/usr/bin/env python
from pprint import pprint
import re


def pvars(_extra: dict = None):
    """Also pass pp(vars()) from inside a def"""
    _vars = {**globals(), **locals(), **(_extra if _extra else {})}
    pprint([[k, _vars[k]] for k in _vars if re.match(r'[a-z]', k)])


def fizz_buzz(i: int) -> str:
    """Get the "fizz" or "buzz" or "fizz buzz" for % 3 and % 5"""
    a = []
    if i % 3 == 0:
        a += ['fizz']
    if i % 5 == 0:
        a += ['buzz']
    if a:
        return ' '.join(a)
    else:
        return str(i)

if __name__ == "__main__":
    import sys
    i = int(sys.argv[1])
    print(i, fizz_buzz(i))
