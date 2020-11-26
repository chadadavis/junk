#!/usr/bin/env python
from pprint import pprint
import re
def pp():
    _vars = { **globals(), **locals() }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])

x=2
y={'k':x, 2:3}

pp()