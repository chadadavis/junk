#!/usr/bin/env python

# TODO
# Link to FreeDictionary for language?
# Allow other dictionaries/langs?
# Option to require full-length matches

from optparse import OptionParser

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
    print(f"{count:3d} {complete} {word}")


