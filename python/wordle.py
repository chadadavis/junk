#!/usr/bin/env python

# https://en.wikipedia.org/wiki/Wordle
# https://www.nytimes.com/games/wordle/index.html

# Backlog:

# Maintain a list of remaining eligible N=5-letter words.
# After each guess, there might be:
#   green letters to filter in (by pos)
#   yellow letters to filter in (anywhere)
#   grey letters to filter out
#   But, grey letters could also be duplicates, don't filter them all out.

# Be smarter about duplicates

# Print top N recommended guesses each iteration
# Score recommended words based on letter freqs, word freqs, and combined

# maintain a dict to filter when:
#   dict(letter in any pos, list)
#      For keeping words, when we find a yellow (elsewhere) letter
#      And for excluding words when we find a grey (nowhere) letter
#   2D lookup for each letter in each pos, from existing words
#     (for when we guess a letter in the right pos):
#     dict(dict(pos, letter))
#  Duplicate letters: need to keep track; don't dump them all together
# Reduce the list based on found constraints.

# Keep track of counts of duplicate letters (don't lump them together).
# From Wikipedia:
# Multiple instances of the same letter in a guess, such as the "o"s
# in "robot", will be colored green or yellow only if the letter also appears
# multiple times in the answer; otherwise, excess repeating letters will be
# colored gray.

# Learn:
# Log previous wordle words, daily:
#   Exclude (recent) past words from guesses, or flag them, with an age, eg 23w (old)
#   What's the distribution of word frequencies of past words. Very frequent words excluded?
#   Are any POS excluded? (e.g. do they include boring pronouns like "their" ?)

# Tests:
# sauce => [caulk]
# others => s(c)(a)ry => [c][a]ndy => [c][a]ped => [c][a]rry => ? => [caulk]

# TODO test dupes, like 'canal'

# Checkout strategy suggestions published by others
# https://slate.com/technology/2022/01/wordle-how-to-win-strategy-crossword-experts.html
# https://www.vulture.com/2022/01/wordle-tips-tricks.html

from optparse import OptionParser
from wordfreq import zipf_frequency

# Generate dict:
#   en-us
#   5-letter words
#   no proper nouns (no initial capitals: Tonya, Tokyo, Timex, Texas, etc)
#   no contractions (e.g. "it'll")
#   only alpha (no accents, e.g. "mêlée")
# =~ 4600 words
# cat /usr/share/dict/american-english |grep '^.....$' |grep -v '^[A-Z]' |grep -v "'" | LANG=C grep '^[a-z]*$' |sort |uniq > english-lower-len-5.dict

LEN = 5
DICT_FILE = './english-lower-len-5.dict'

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


# Collect indexes to find words matching certain criteria.
# This is a dict[list[set[]]]
# The dict tracks the letters
# The list tracks what position the letter is in (0 for wildcard/yellow position)
# The set tracks the words that meet that criteria.
# So, the [0] set of words is all the words that have the letter at any position.
# candidates['y'][0] contains 'word' # means the letter is in the word
# candidates['y'][2] contains 'word' # means the letter is the second letter in the word

lookup = dict()

# Build starting list
file = open(DICT_FILE)
count = 0

for word in file:
    word = word.strip()
    count += 1

    for pos, letter in enumerate(word):
        lookup[letter]    = lookup.get(letter) or [ set() for i in range(LEN+1) ]
        # Note, this word is a candidate for (yellow/wildcard) letter, pos 0
        # lookup[letter][0] = lookup[letter][0] or set()
        # lookup[letter][0].add(word)

        # Note, this word is a candidate for (green, positioned) letter (1-based)
        lookup[letter][pos+1].add(word)

parser = OptionParser()
parser.add_option("-d", "--dictionary", dest="DICT_FILE",
    help="Dictionary file",
)

(options, args) = parser.parse_args()

# The remaining args are previous guesses, if any
# TODO DEL tests ...
# args = args or ['s-c+a*r-y-']

# The syntax/encoding for the response to each previous guess response looks like eg:
#   c+a*n-a-l*
# Which means (0-based index of these chars):
#   0-1 There's a 'c' at pos 1 (and maybe elsewehere?)
#   2-3 There's an (or more) 'a' but not at pos 2 (pos 2 out of 5, 1-based)
#   4-5 There is no 'n'
#   6-7 There are no *additional* 'a' letters (i.e. just the previously found)
#   8-9 There's an (or more) 'l', but not at pos 5

blacklist_words = set()
required_letters = set()
for guess in args:
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

# Go over remaining candidates and score them
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

# TODO sort by whichever score
for word in remaining:
    # Word freq
    word_score = zipf_frequency(word, 'en')

    # Sum of freq of (unique) letters
    letter_score = score_letters(word)

    # A slightly more comparable scale:
    combined_score = 10 * letter_score + word_score

    print(f"#{count:04d} {letter_score:5.2f} {word_score:5.2f} {combined_score:5.2f} {word:20s}")

print()
