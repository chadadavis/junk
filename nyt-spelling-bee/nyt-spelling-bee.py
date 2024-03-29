#!/usr/bin/env python

import os
import random
import re
import readline
import subprocess
import textwrap
from operator import itemgetter
from optparse import OptionParser

from wordfreq import zipf_frequency

# TODO backlog:

# TODO optparse deprecated, switch to argparse

# NYT spelling bee, make it iteratively expose hints, by showing a count of how
# many target words begin with the prefix : "a... (7)" then "as... (5)" for each
# of the letters.
# Format into a bulleted list?
#
# Optionally: once only one word left ("clar..."), but don't expose the last few chars:
# a hint that exposes the definition (but filter the word
# out of the def)
# See eg https://www.nytimes.com/2022/11/08/crosswords/spelling-bee-forum.html


def is_valid_word(word, alphabet):
    word = word.lower()
    for letter in word:
        if letter not in alphabet:
            return False
    return True


def is_complete_word(word, alphabet_full):
    word = word.lower()
    for letter in alphabet_full:
        if letter not in word:
            return False
    return True


def beep():
    print("\a", end='', flush=True)


def print_alphabet(alphabet_part, alphabet_req):
    alphabet_req = alphabet_req or ' '
    alphabet_part = list(alphabet_part.upper())

    # If there is a required letter, insert it in the middle of the hexagon
    alphabet_part.insert(3, alphabet_req.upper())
    hex = ''\
        + ' {} {}   \n'\
        + '{} {} {} \n'\
        + ' {} {}   \n'\
        + ''
    print(hex.format(*alphabet_part))


scores = dict()

def completer(text: str, state: int) -> str:
    """Readline (TAB) autocompletion of remaining candidate words"""
    global scores
    global opts
    if opts.play:
        return
    completions = []
    if not text:
        return

    # Completions via recent spellcheck suggestions (from last online fetch)
    completions += [
                    s for s in scores
                    # Only complete words not yet processed
                    if not scores[s]['status'] and s.casefold().startswith(text.casefold())
                    ]

    if state < len(completions):
        return completions[state]

    if state == 0:
        # text doesn't match any possible completion
        beep()


def wrapper(string):
    LINE_WIDTH = os.get_terminal_size().columns
    WRAP_WIDTH = LINE_WIDTH // 2

    lines_wrapped = []
    for line in string.splitlines():
        line_wrap = textwrap.wrap(line, WRAP_WIDTH, replace_whitespace=False, drop_whitespace=True)
        line_wrap = line_wrap or ['']
        lines_wrapped += line_wrap
    string = "\n".join(lines_wrapped)
    return string


def create_word(word: str, alphabet_full: str, complete=''):

    # Use an inverse frequency to give more 'difficulty' points for less common words.
    freq = 100 / (zipf_frequency(word, 'en') or 1)
    # This puts two scores on a (roughly) 1-100 scale
    l = 10 * len(word)
    # And (equally weighed) average them
    difficulty = (freq + l) / 2
    if not complete:
        if is_complete_word(word, alphabet_full):
            complete = '*'

    obj = {
        'word': word,
        'freq': freq,
        'l': l,
        'difficulty': difficulty,
        'status': complete,
    }
    return obj


readline.set_completer(completer)
readline.parse_and_bind("tab: complete")

opt_parser = OptionParser()
opt_parser.add_option('-r', '--req',      help="A letter that is required in every word. Optional.")
opt_parser.add_option('-m', '--min',      help="Minimum length of valid words. Default 4", default=4, type='int')
opt_parser.add_option('-p', '--pn',       help="Include proper nouns, title case words (eg names). Default 0", default=0, type='int')
opt_parser.add_option('-d', '--dict',     help="Dictionary name (/usr/share/dict/*) or full path. Default 'american-english'", default='american-english')
opt_parser.add_option('--play', action='store_true', help="Play against this script, hiding candidate words until guessed.")

(opts, args) = opt_parser.parse_args()
if not args:
    opt_parser.print_usage()
    exit(1)

alphabet_req = (opts.req or "").lower()
alphabet_part = args[0].lower()
alphabet_full = alphabet_part

if alphabet_req and alphabet_req not in alphabet_full:
    alphabet_full += alphabet_req

if not opts.dict.startswith('/'):
    opts.dict = '/usr/share/dict/' + opts.dict

file = open(opts.dict)
count = 0

for word in file:
    word = word.strip()
    if opts.pn == 0 and word.istitle():
        continue
    if len(word) < opts.min:
        continue
    if alphabet_req and alphabet_req not in word:
        continue
    if not is_valid_word(word, alphabet_full):
        continue
    count += 1

    scores[word] = create_word(word, alphabet_full)


definition = None
while True:
    # Clear screen
    # print('\033c')
    print()
    # Headings
    print(f"{'#':>2} {'len':>4} {'rare':>4} {'comb':>4} {'*':>1} {'word'}\n")
    # Remaining words, sorted, by status, and then by difficulty (since sorted() is stable)
    scores_sorted = scores.values()
    scores_sorted = sorted(scores_sorted, key=itemgetter('difficulty'), reverse=False)
    scores_sorted = sorted(scores_sorted, key=itemgetter('status'),     reverse=False)

    missing_n = 0
    for i, s in enumerate(scores_sorted):
        if opts.play and ( not s['status'] or s['status'] == '*' ):
            missing_n += 1
            continue
        print(f"{i:2d} {s['l']:4.0f} {s['freq']:4.0f} {s['difficulty']:4.0f} {s['status']:>1s} {s['word']:20s}")
    print()

    if opts.play:
        print(f"{missing_n:3d} words to go\n")

    print_alphabet(alphabet_part, alphabet_req)

    # All words covered?
    if len( [ k for k in scores if not scores[k]['status'] ]    ) == 0:
        exit()

    if definition is not None:
        print(wrapper(definition), "\n")
        definition = None

    # Mark words as accepted / rejected, while any left (without status)
    try:
        cmd = input("cmd [? + - *]: ")
    except:
        print()
        exit()

    match = re.match(r'\s*([?+*-]?)\s*(.*)\s*', cmd)
    if not match:
        beep()
        continue
    op, word = match.groups()

    if op == '*':
        # (re-)randomize the alphabet (for the printed display each iteration).
        # This helps with visually guessing words.
        alphabet_part = ''.join(random.sample(alphabet_part, len(alphabet_part)))
        continue

    if word in scores:
        # The default operator is to mark the word as already accepted
        op = op or '+'
        if op in ('-', '+'):
            if scores[word]['status'] == op:
                # Already did this one
                beep()
            scores[word]['status'] = op
    else:
        beep()
        if op == '+':
            # But allow a preceding '+' to force acceptance of an unknown word.
            # Because the NYT dict has some words that our dict doesn't
            scores[word] = create_word(word, alphabet_full, '+')

    # Lookup definition of a word
    if op == '?':
        # This sed command strips off the first two header lines from the output
        # And the ' ... || true' makes this actually *not* check the return code for success,
        # since no definition is also fine.
        definition = subprocess.check_output(f'dict -f -d wn {word} 2>/dev/null|sed -e1,2d|xargs 2>/dev/null || true', shell=True)
        definition = definition.decode('utf-8')
        definition = definition.strip()
        definition = definition[:200]


