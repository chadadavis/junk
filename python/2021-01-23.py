#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])

exits = {
    0: {'Q': 0},
    1: {'Q': 0, 'W': 2, 'E': 3},
    2: {'Q': 0, 'N': 5},
    3: {'Q': 0, 'W': 1},
    }
