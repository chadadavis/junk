#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])


import readline

opts = ['angela@domain.com', 'mikey@smack.com', 'michael@domain.com', 'david@test.com']

def completer(text, state):
    options = [x for x in opts if x.startswith(text)]
    try:
        return options[state]
    except IndexError:
        return None

readline.set_completer(completer)
readline.parse_and_bind("TAB: complete")

while 1:
    a = input("> ")
    print("You entered", a)

