#!/usr/bin/env python
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pp(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])


import readline

# For accent-insensitive completion
import unidecode

opts = ['angela@domain.com', 'mikey@smack.com', 'michael@domain.com', 'david@test.com']

def completer(text: str, state: int) -> str:
    completions = []

    # Allow global completion of the whole dictionary before any text?
    # if not text:
    #     return

    # Unidecode allows accent-insensitive autocomplete
    ud = unidecode.unidecode
    text = ud(text)

    # # Completions via readline history
    # for i in range(1, readline.get_current_history_length() + 1):
    #     i = readline.get_history_item(i)
    #     if ud(i).casefold().startswith(text.casefold()):
    #         completions += [ i ]

    # Autocomplete via prefix search
    for i in opts:
        if ud(i).casefold().startswith(text.casefold()):
            completions += [ i ]

    if state < len(completions):
        try:
            return completions[state]
        except IndexError:
            return None


    if state == 0:
        # text doesn't match any possible completion
        # eg beep()
        ...


readline.set_completer(completer)
readline.parse_and_bind("TAB: complete")

# Allow to complete multi-token terms (with spaces)
readline.set_completer_delims('')

while 1:
    a = input("> ")
    print("You entered", a)

