#!/usr/bin/env python

# Go through whole dictionary, save only N=5-letter words
# Keep a backlog of possible words left, print top few each iteration
# Score word based on letter freqs and word freqs (combined)
#  They seem to also avoid obscure words
#  But, strategically obscure words with high letter freqs might be informative
#  So, maybe print two lists, with top words for guessing letters, and separately for top letters (left)

# maintain a dict to filter when:
#   dict(letter in any pos, list)
#      For keeping words, when we find a yellow (elsewhere) letter
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
# DICT_FILE = '/usr/share/dict/american-english'
DICT_FILE = './english-lower-len-5.dict'

def score_letters(word):
    score = 0.0
    used = set()
    for letter in word:
        # But don't double count duplicate letters since they're uninformative
        if letter.lower() in used:
            continue
        score += letter_freq[letter.lower()]
        used.add(letter.lower())
    return score

parser = OptionParser()
parser.add_option("-d", "--dictionary", dest="DICT_FILE",
    help="Dictionary file",
)

(options, args) = parser.parse_args()

# if not args:
#     parser.print_usage()
#     exit(1)

letter_freq = {
    'e': 0.13000,
    't': 0.09100,
    'a': 0.08200,
    'o': 0.07500,
    'i': 0.07000,
    'n': 0.06700,
    's': 0.06300,
    'h': 0.06100,
    'r': 0.06000,
    'd': 0.04300,
    'l': 0.04000,
    'c': 0.02800,
    'u': 0.02800,
    'm': 0.02500,
    'w': 0.02400,
    'f': 0.02200,
    'g': 0.02000,
    'y': 0.02000,
    'p': 0.01900,
    'b': 0.01500,
    'v': 0.00980,
    'k': 0.00770,
    'j': 0.00150,
    'x': 0.00150,
    'q': 0.00095,
    'z': 0.00074,
}


file = open(DICT_FILE)
count = 0
for word in file:
    word = word.strip()
    count += 1

    # Word freq
    word_score = zipf_frequency(word, 'en')

    # Sum of freq of (unique) letters
    letter_score = score_letters(word)

    # A slightly more comparable scale:
    combined_score = 10 * letter_score + word_score

    print(f"#{count:04d} {letter_score:5.2f} {word_score:5.2f} {combined_score:5.2f} {word:20s}")
