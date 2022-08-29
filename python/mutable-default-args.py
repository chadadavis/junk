#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])

import time

# Mutable default arg, is eval'd only once, during the def
def f(x=[]):
    # This body is eval'd on every invocation, so this is where a fresh default mutable
    # must be created.
    x = x or []

    time.sleep(1)
    x.append(int(time.time()))
    return x

a = f()
print(a)

b = f()
print(b)
