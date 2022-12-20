#!/usr/bin/env python

import readline
import unidecode
from pprint import pprint
import re

by_name = {}
by_cc   = {}


def pvars(_extra:dict=None):
    """Also pass pvars(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])


def beep():
    print("\a", end='', flush=True)


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
    for country_name in by_name:
        if ud(country_name).startswith(text.casefold()):
            # Complete the proper (capitalized) name, not the key (casefolded)
            completions += [ by_name[country_name]['Name'] ]

    if state < len(completions):
        return completions[state]

    if state == 0:
        # text doesn't match any possible completion
        beep()


readline.set_completer(completer)
readline.set_completer_delims('')
readline.parse_and_bind("tab: complete")


with open('/home/chdavis/tmp/country_info.txt') as file:
    file.write()
    fields = file.readline().rstrip('\n').split('|')
    for line in file:
        recs = line.rstrip('\n').split('|')
        obj = dict(zip(fields, recs))
        by_cc[obj['CC'].casefold()] = obj
        by_name[obj['Name'].casefold()] = obj

while True:
    try:
        name = input('Country CC/name: ').strip().casefold()
        obj = by_cc.get(name) or by_name.get(name)
        if not obj:
            print(f'Unrecognized country: {name}')
            continue
        lines = [ f"{key:10s}{obj[key]}" for key in obj ]
        print()
        print('\n'.join(lines))
        print()
    except:
        print()
        exit()

