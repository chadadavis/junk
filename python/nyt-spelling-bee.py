#!/usr/bin/env python

# Backlog:
# Find/Allow other dictionaries/langs. Rather, check/dedupe multiple dicts.
# Option to require full-length matches.
# Generate puzzles, or find more easier/harder ones.

import subprocess
from optparse import OptionParser
from wordfreq import zipf_frequency
from operator import itemgetter
import re
import readline


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


def beep():
    print("\a", end='', flush=True)


scores = dict()
def completer(text: str, state: int) -> str:
    """Readline (TAB) autocompletion of remaining candidate words"""
    global scores

    completions = []
    if not text:
        return

    # Completions via recent spellcheck suggestions (from last online fetch)
    completions += [
                    s for s in scores
                    # Only complete words not yet processed
                    if not scores[s]['status'] and s.startswith(text.casefold())
                    ]

    if state < len(completions):
        return completions[state]

    if state == 0:
        # text doesn't match any possible completion
        beep()


readline.set_completer(completer)
readline.parse_and_bind("tab: complete")


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
    complete = '*' if is_complete_word(word, alphabet) else ''

    # This puts two scores on a (roughly) 1-100 scale
    l = 10 * len(word)
    # Use an inverse frequency to give more 'difficulty' points for less common words.
    freq = 100 / (zipf_frequency(word, 'en') or 1)
    # And (equally weighed) average them
    difficulty = (freq + l) / 2

    scores[word] = {
        'word': word,
        'freq': freq,
        'l': l,
        'difficulty': difficulty,
        'status': complete,
    }


while True:
    # Headings
    print(f"{'#':>2} {'len':>4} {'freq':>4} {'comb':>4} {'*':>1} {'word'}")
    # Remaining words, sorted, by status+difficulty
    scores_sorted = scores.values()
    scores_sorted = sorted(scores_sorted, key=itemgetter('difficulty'), reverse=False)
    scores_sorted = sorted(scores_sorted, key=itemgetter('status'),     reverse=False)

    for i, s in enumerate(scores_sorted):
        print(f"{i:2d} {s['l']:4.0f} {s['freq']:4.0f} {s['difficulty']:4.0f} {s['status']:>1s} {s['word']:20s}")
    print()

    # All words covered?
    if len( [ k for k in scores if not scores[k]['status'] ]    ) == 0:
        exit()

    # Mark words as accepted / rejected, while any left (without status)
    try:
        cmd = input("cmd [? + -]: ")
    except:
        print()
        exit()

    match = re.match('\s*([?+-]?)\s*(.+)\s*', cmd)
    if not match:
        beep()
        continue
    op, word = match.groups()
    if word not in scores:
        beep()
        continue

    # The default operator is to mark he word as already accepted
    op = op or '+'
    if op in ('-', '+'):
        scores[word]['status'] = op

    # Lookup definition of a word
    if op == '?':
        # This sed command strips off the first two header lines from the output
        # And the ' ... || true' makes this actually *not* check the return code for success,
        # since no definition is also fine.
        definition = subprocess.check_output(f'dict -f -d wn {word} 2>/dev/null|sed -e1,2d|xargs 2>/dev/null || true', shell=True)
        definition = definition.decode('utf-8')
        definition = definition.strip()
        definition = definition[:200]
        print("\n", definition, "\n")

