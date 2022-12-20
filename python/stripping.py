#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])

import os
ENV = os.environ

import os.path

# path = os.path.join(ENV['HOME'], '.bashrc')
with open('f.tsv') as fh:
    # If you use the default, it'll also remove any trailing whitespace on the line.
    # That could break files formatted with whitespace fields, eg TSV files
    # x = fh.readline().rstrip()
    # So, just strip the newline only
    x = fh.readline().rstrip('\n')
    # TSV:
    fields = x.split('\t')
    print(fields)

