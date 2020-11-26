#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])


def f(x:int=2, y:float=3.0) -> int:
    """Uselessness"""
    z = x * y
    x=3
    pvars(vars())
    return z

xn = 1
yn = 5.0

zn = f(xn, yn)

pvars()
