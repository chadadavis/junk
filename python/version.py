#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])


import sys

print(sys.version_info)
