#!/usr/bin/env python

# Go through whole dictionary, save only N=5-letter words
# Keep a backlog of possible words left, print top few each iteration
# Score word based on letter freqs and word freqs (combined)
#  They seem to also avoid obscure words
#  But, strategically obscure words with high letter freqs might be informative
#  So, maybe print two lists, with top words for guessing letters, and separately for top letters (left)

# maintain a dict to filter when:
#   dict(letter in any pos, list)
#      For keeping words, when we find a yellow (elsewhere) lettter
#      And for excluding words when we find a grey (nowhere) letter
#   2D lookup for each letter in each pos, from existing words
#     (for when we guess a letter in the right pos):
#     dict(dict(pos, letter))
#  Duplicate letters: test it, but should be automatic
# Reduce the list based on found constraints.



# Backlog:
# Look at letter frequencies by position (in 5-letter-word) as well

# Find/Allow other dictionaries/langs. Rather: check/dedupe multiple dicts.

import subprocess
from optparse import OptionParser
from wordfreq import zipf_frequency
# import letter_frequency_languages


# DICT_FILE = '/usr/share/dict/british-english'
# DICT_FILE = '/usr/share/dict/cracklib-small'
DICT_FILE = '/usr/share/dict/american-english'

LEN = 5

parser = OptionParser()
parser.add_option("-r", "--required", dest="required",
    help="A letter that is required in every partial anagram",
)

(options, args) = parser.parse_args()
if not args:
    parser.print_usage()
    exit(1)

letter_freq = {
    'E': 0.13000,
    'T': 0.09100,
    'A': 0.08200,
    'O': 0.07500,
    'I': 0.07000,
    'N': 0.06700,
    'S': 0.06300,
    'H': 0.06100,
    'R': 0.06000,
    'D': 0.04300,
    'L': 0.04000,
    'C': 0.02800,
    'U': 0.02800,
    'M': 0.02500,
    'W': 0.02400,
    'F': 0.02200,
    'G': 0.02000,
    'Y': 0.02000,
    'P': 0.01900,
    'B': 0.01500,
    'V': 0.00980,
    'K': 0.00770,
    'J': 0.00150,
    'X': 0.00150,
    'Q': 0.00095,
    'Z': 0.00074,
}

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

    print(f"{count:3d} {freq:3.0f} {l:3.0f} {difficulty:3.0f} {complete:1s} {word:20s} {definition:s}")
