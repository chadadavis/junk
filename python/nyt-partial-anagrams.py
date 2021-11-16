#!/usr/bin/env python

# TODO
# Note (*) which have all letters, if any
# Count number of items, just number the output
# Use OptionParser to note order of args

import sys
from pprint import pprint
import re

def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])

from optparse import OptionParser
parser = OptionParser()
(options, args) = parser.parse_args()


# letters = 'marjorly'
# required = 'y'

# letters = 'modehut'
# required = 'h'

alphabet = (args and args[0]) or "marjorl"
required = (args and args[1]) or "y"

if required not in alphabet:
    alphabet += required

print(",".join(alphabet))

def validate_word(word, alphabet):
    for letter in word:
        if letter not in alphabet:
            return False
    return True

file = open('/usr/share/dict/american-english')
for word in file:
    word = word.strip()
    if len(word) < 4:
        continue
    if required not in word:
       continue
    if not validate_word(word, alphabet):
        continue
    print(word)

