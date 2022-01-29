#!/usr/bin/env python

# Backlog:
# Find/Allow other dictionaries/langs. Rather, check/dedupe multiple dicts.
# Option to require full-length matches.
# Generate puzzles, or find more easier/harder ones.

import subprocess
from optparse import OptionParser
from wordfreq import zipf_frequency

# DICT_FILE = '/usr/share/dict/british-english'
# DICT_FILE = '/usr/share/dict/cracklib-small'
DICT_FILE = '/usr/share/dict/american-english'

MIN_LEN = 4

parser = OptionParser()
parser.add_option("-r", "--required", dest="required",
    help="A letter that is required in every partial anagram",
)

(options, args) = parser.parse_args()
if not args:
    parser.print_usage()
    exit(1)

alphabet = args[0]
required = options.required or ""

if required and required not in alphabet:
    alphabet += required


def is_valid_word(word, alphabet):
    for letter in word:
        if letter not in alphabet:
            return False
    return True


def is_complete_word(word, alphabet):
    for letter in alphabet:
        if letter not in word:
            return False
    return True


file = open(DICT_FILE)
count = 0
for word in file:
    word = word.strip()
    if len(word) < MIN_LEN:
        continue
    if required and required not in word:
        continue
    if not is_valid_word(word, alphabet):
        continue
    count += 1
    complete = '*' if is_complete_word(word, alphabet) else ' '

    # This puts two scores on a (roughly) 1-100 scale
    l = 10 * len(word)
    # Use an inverse frequency to give more 'difficulty' points for less common words.
    freq = 100 / (zipf_frequency(word, 'en') or 1)
    # And (equally weighed) average them
    difficulty = (freq + l) / 2

    # This sed command strips off the first two header lines from the output
    # And the ' ... || true' makes this actually *not* check the return code for success,
    # since no definition is also fine.
    definition = subprocess.check_output(f'dict -f -d wn {word} 2>/dev/null|sed -e1,2d|xargs 2>/dev/null || true', shell=True)
    definition = definition.decode('utf-8')
    definition = definition.strip()
    definition = definition[:200]
    print(f"{count:3d} {freq:3.0f} {l:3.0f} {difficulty:3.0f} {complete:1s} {word:20s} {definition:s}")
