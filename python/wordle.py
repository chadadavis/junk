#!/usr/bin/env python

# https://www.nytimes.com/games/wordle/index.html
# https://en.wikipedia.org/wiki/Wordle

# Backlog:
# Search for TODO below

# Option to test against a given target word arg

# Option to play, where a random target is chosen, but not revealed
# User is prompted for guesses

# Option to autoplay, where random target is chosen, and then iterates itself.
# Guesses just based on the #1 ranked word (optimize that later ...)

# Learn:
# Log previous wordle words, daily:
#   Exclude (recent) past words from guesses, or flag them, with an age, eg 23w (old)
#   What's the distribution of word frequencies of past words, relative to the dictionary?
#   Any words have been repeated on multiple days?
#   Are any PoS excluded? (e.g. do they include boring pronouns like "their" ?)
#   Any bias toward PoS (adjectives), or against PoS (plural nouns) ?
# Consider other dictionaries. Eg 'trove' (2022-02-23) isn't in any of /usr/share/dict/*

# TODO Be smarter about duplicates

# Regarding duplicates: (From Wikipedia):
# Multiple instances of the same letter in a guess, such as the "o"s
# in "robot", will be colored green or yellow only if the letter also appears
# multiple times in the answer; otherwise, excess repeating letters will be
# colored gray (and it's *not* sequential, could have gray 'o' before green 'o')

# Checkout strategy suggestions published by others:
# https://slate.com/technology/2022/01/wordle-how-to-win-strategy-crossword-experts.html
#   Alt strategy: maximize information gain of guess by not always including required letters.
#   What choice of letters gets closest to a 50% split of eligible words (max elimination rate)

#   'According to our analysis, almost exactly one-third of Wordle solutions
#   contain at least one duplicated letter—this is worth keeping in mind,
#   regardless of your strategy.

import readline  # Not referenced, but used by input()
import random
from optparse import OptionParser
from operator import itemgetter
from wordfreq import zipf_frequency

def score_letters(word):
    """For choosing candidate starting words, based on common letter frequencies"""

    score = 0.0
    used = set()
    for letter in word:
        # But don't double count duplicate letters since they're uninformative
        if letter.lower() in used:
            continue
        score += letter_freq[letter.lower()]
        used.add(letter.lower())
    return score


# TODO: update with one that has a proper/recent citation
# https://en.wikipedia.org/wiki/Letter_frequency
# or try something like: import letter_frequency_languages
# And Look at letter frequencies by position (in 5-letter-word), not just globally
# TODO just derive this from our own dictionary
# TODO maybe also useful: track the 'specificity' of each letter (ability to exclude max words)
# E.g. score a word based on having letters whose freq (in our dict) is closest to 50% ?
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

parser = OptionParser()
parser.add_option('--length',     type='int',          help="Length of all words, default 5", default=5)
parser.add_option('--target',     type='string',       help="Set the target word, e.g. to test")
parser.add_option('--random',     action='store_true', help="Pick a random target, for you to play locally. Else assume unknown.")
parser.add_option('--auto',       action='store_true', help="The algorithm plays against itself.")
parser.add_option('--dict',       type='string',       help="Path to custom dictionary file, one word per line",
    default='./english-lower-len-5.dict', # TODO generalize the path
    )
(options, args) = parser.parse_args()

LEN = options.length

# Collect indexes to find words matching certain criteria.
# This is a dict[list[set[]]]
# The dict tracks the letters
# The list tracks what position the letter is in (0 for wildcard/yellow position)
# The set tracks the words that meet that criteria.
# candidates['y'][2] contains 'word' # means the letter is the second letter in the word

lookup = dict()

# Build starting list
# TODO: remove custom file and just build this based on the standard en-us dictionary
# Generate dict (based on what I assume Wordle uses)
#   en-us
#   5-letter words (based on LEN option)
#   no proper nouns (no initial capitals: Tonya, Tokyo, Timex, Texas, etc)
#   no contractions (e.g. "it'll")
#   only ASCII (no accents, e.g. "mêlée")
# =~ 4600 words
# cat /usr/share/dict/american-english |grep '^.....$' |grep -v '^[A-Z]' |grep -v "'" | LANG=C grep '^[a-z]*$' |sort |uniq > english-lower-len-5.dict

file = open(options.dict)
num_words = 0

for word in file:
    num_words += 1
    word = word.strip()
    for pos, letter in enumerate(word):
        lookup[letter]    = lookup.get(letter) or [ set() for i in range(LEN+1) ]

        # Note, this word is a candidate for (green, positioned) letter (1-based)
        lookup[letter][pos+1].add(word)

if not options.target and options.random:
    # To get a random word, seek back to the right pos in file
    i = random.randint(0, num_words-1)
    file.seek(i * (LEN+1)) # Fixed line width, plus trailing "\n" byte
    options.target = file.readline()
    options.target = options.target.strip()

file.close()

# This basically implements hard-mode by default, where confirmed letters are subsequently required.
required_letters = set()
blacklist_words = set()

while True:
    # The syntax/encoding for the response to each previous guess response looks like eg:
    #   c+a*n-a-l*
    # Which means (0-based index of these chars):
    #   0-1 There's a 'c' at pos 1 (and maybe elsewehere?)
    #   2-3 There's an (or more) 'a' but not at pos 2 (pos 2 out of 5, 1-based)
    #   4-5 There is no 'n'
    #   6-7 There are no *additional* 'a' letters (i.e. just the previously found)
    #   8-9 There's an (or more) 'l', but not at pos 5

    # TODO skip input() if autoplay
    guess = input("Previous reply (e.g. 's+o-l*i*d-'): ")

    # Note if a letter was never seen (and no later positions), as then 'grey' means not present at all
    letter_maybe_del = set()
    for pos in range(LEN):
        letter = guess[pos*2]
        op     = guess[pos*2+1]

        # Note, the below [pos+1] syntax is because 1-based counting in the target word
        if op == '+':
            # Letter is present, at this position.
            required_letters.add(letter)
            # But maybe also at other positions ... so, don't delete those yet.
            ...
            # However, no *other* letter is at *this* pos, so delete all of those.
            for l in lookup:
                if l != letter:
                    blacklist_words = blacklist_words | lookup[l][pos+1]
                    lookup[l][pos+1] = set()
        if op == '*':
            # Letter is still a candidate, but not at this pos.
            # Might still have (multiple) occurrences elsewhere.
            blacklist_words = blacklist_words | lookup[letter][pos+1]
            lookup[letter][pos+1] = set()
            # This letter is now required at some/any other pos
            required_letters.add(letter)
        if op == '-':
            # Letter is not present in this position
            blacklist_words = blacklist_words | lookup[letter][pos+1]
            lookup[letter][pos+1] = set()

            # Letter *maybe* not present at any other position, but maybe *later* in the word:
            # (Apparently the green letters take priority over the yellow/grey letters).
            letter_maybe_del.add(letter)
            # If letter has no (more) occurrences (including *later* occurrences):
            # Counter-example: target 'shake', but given 's+e-r-v-e+' (clearly not sequential)
            # Because the 'e' in pos 2 is grey, even before the (green) 'e' at pos 5 was processed.

    # After checking each pos for each letter, which were grey once, but otherwise not green later:
    for letter in letter_maybe_del:
        # Did we then find it later as a '*' or '+' letter?
        if letter not in required_letters:
            # Then blacklist all those words with this letter anywhere
            for s in lookup[letter]:
                blacklist_words = blacklist_words | s
            lookup[letter] = [ set() for i in range(LEN+1)]

    # Go over remaining candidates
    remaining = set()
    for letter in lookup:
        for pos in range(LEN):
            remaining = remaining | lookup[letter][pos+1]
            # Really not efficient, but we need to ensure that wildcard letters present:
            for w in lookup[letter][pos+1]:
                for l in required_letters:
                    if l not in w:
                        blacklist_words.add(w)

    remaining = remaining - blacklist_words

    scores = dict()
    for word in remaining:
        # Word freq. Note, this just makes the word more likely to be in the Wordle
        # dictionary, since there's some threshold for excluding less common words.
        # However, given two words in the dictionary, that one is twice as frequent
        # as the other doesn't mean it's more likely to be the target word.
        by_word = zipf_frequency(word, 'en')

        # Sum of freq of (unique) letters
        by_letter = score_letters(word)

        # A slightly more comparable scale:
        by_combined = 10 * by_letter + by_word

        scores[word] = { 'word': word, 'by_word': by_word, 'by_letter': by_letter, 'by_combined': by_combined, }

    # Sort for top N objs by key
    scores = sorted(scores.values(), key=itemgetter('by_combined'), reverse=True)
    for s in scores[:10]:
        print(f"{s['by_letter']:5.2f} {s['by_word']:5.2f} {s['by_combined']:5.2f} {s['word']:20s}")

