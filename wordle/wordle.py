#!/usr/bin/env python3

# This is a script to play, and to help you play and win at, Wordle.
# https://www.nytimes.com/games/wordle/index.html
# https://en.wikipedia.org/wiki/Wordle

# A note regarding duplicate letters: (From Wikipedia):
# Multiple instances of the same letter in a guess, such as the "o"s
# in "robot", will be colored green or yellow only if the letter also appears
# multiple times in the answer; otherwise, excess repeating letters will be
# colored gray (and it's *not* sequential, could have gray 'o' before green 'o')

# Backlog:
# Or, Search for TODO below in the code

# TODO optparse deprecated, switch to argparse

# There is a list of potential target words (the A list), which seems to exclude declensions.
#   Use a PoS tagger to boost words that are equal to their own stem? (And non-proper-nouns, etc)
# There is a list of allowed guesses (the B list), which can be strategically useful to excluding letters.
# Use a decision tree / random forest?
# https://en.wikipedia.org/wiki/Random_forest

# Scoring
# Once I've already confirmed an S (maybe at a single pos),
# the remaining scores for other potential S letters in the word should be updated
# Is an HMM relevant here?

# eg slate=>price=>whine=>[guide,oxide]

# At the last iteration, 'guide' isn't more likely than 'oxide', they're equal. One
# can only say that guide is more likeley than oxide based on the fact that it
# contains letters that are more frequent in the dictionary as a whole. But, by
# the time we've eliminated all the other words, the last two words are equally
# likely to be the answer. It's just hypothetical to score "guide" higher, but not real.

# Consider not requiring hard-mode always?
# But that changes a lot of the assumptions that each reply contains all the inforamtion about subsequent candidates ...

# TODO Auto mode: (with stats)
# Make it into a benchmark mode, that loops over the whole dictionary, and compute the average num guesses.
# So, that you can then evaluate alternative strategies/scoring across the whole dictionary.
# cf. https://freshman.dev/wordle/#/leaderboard
# And https://www.reddit.com/r/wordle/comments/s88iq4/a_wordle_bot_leaderboard/
# And make it print a running average while it's running ?

# Reimplement the algo to use an approach where the letters/words are chosen based on maximizing the split.
# Compare to NYT WordleBot https://www.nytimes.com/2022/04/07/upshot/wordle-bot-introduction.html
# Then display a count of the number of words eliminated per round, along with the count of remaining words

# TODO be more efficient with processing of duplicates

# Checkout strategy suggestions published by others:
# https://slate.com/technology/2022/01/wordle-how-to-win-strategy-crossword-experts.html
#   Alt strategy: maximize information gain of guess by not always including required letters.
#   What choice of letters gets closest to a 50% split of eligible words (max elimination rate)


import os
import random
import re
import readline
from operator import itemgetter
from optparse import OptionParser


def score_letters(word):
    """For choosing candidate starting words, based on common letter frequencies"""
    global opts
    global letter_freq_d
    score = 0.0
    used = set()
    for pos, letter in enumerate(word):

        letter = letter.lower()

        if opts.scoring == 2 and letter not in used:
            # Based on whatever dictionary we read in from opts.dict
            score += letter_freq_d[letter][0]
        elif opts.scoring == 3 and letter not in used:
            # Based on minimizing distance of each score from 50% (discriminatory ability)
            score += 0.5 - abs(0.5 - letter_freq_d[letter][0])
        elif opts.scoring == 4:
            # Also count per-pos frequency (higher prio)
            # Pos is 1-based counting, since [0] counts "any position"
            score += 0.5 - abs(0.5 - letter_freq_d[letter][pos+1])

            # TODO rather than completely skip duplicate scoring, we could just linear downweight duplicates.
            # Because words do have duplicate letters, and we might get extra info by guessing for duplicates too.
            if letter not in used:

                # Scale this down, since per-pos score weighs more
                score += 0.05 * (0.5 - abs(0.5 - letter_freq_d[letter][0]))

        used.add(letter)
    return score


def beep():
    print("\a", end="", flush=True)


def completer(text: str, state: int):
    """Readline (TAB) autocompletion of remaining candidate words"""
    global words_left

    completions = []
    if not text:
        return

    # Completions via recent spellcheck suggestions (from last online fetch)
    completions += [
                    s for s in words_left
                    if s.startswith(text.casefold())
                    ]

    if state < len(completions):
        return completions[state]

    if state == 0:
        # text doesn't match any possible completion
        beep()


print("\nTry instead:")
print("https://replit.com/@chadadavis/Wordle-Whittler")

readline.set_completer(completer)
readline.parse_and_bind("tab: complete")

base_dir = os.path.dirname(os.path.abspath(__file__))
dict_file_path = os.path.join(base_dir, 'wordle-a.dict')

parser = OptionParser()
parser.add_option('--top',        type='int',          help="Show top N=20 candidates each round", )
parser.add_option('--length',     type='int',          help="Length of all words, default 5", default=5)
parser.add_option('--scoring',    type='int',          help="Scoring mode", default=4)
parser.add_option('--target',     type='string',       help="Set the target word, e.g. to test")
parser.add_option('--start',      type='string',       help="Set the start word, e.g. to test")
parser.add_option('--random',     action='store_true', help="Pick a random target, for you to play locally. Else assume unknown.")
parser.add_option('--auto',       action='store_true', help="The algorithm plays against itself.")
parser.add_option('--dict',       type='string',       help="Path to custom dictionary file, one word per line",
    default=dict_file_path,
    )
parser.add_option('--boost',      type='string',       help="File containing words that are more likely to be picked",
    default=dict_file_path
    )
(opts, args) = parser.parse_args()
# Default to printing 0 choices in auto mode
# Else based on terminal height (if not already explicitly defined)
top_default = max(10, int(os.get_terminal_size().lines *.9))
opts.top = opts.top if opts.top is not None else (0 if opts.auto else top_default)
opts.random = opts.random or opts.auto
LEN = opts.length

# Build starting list from dictionary
# TODO: remove custom file and just build this based on the standard en-us dictionary
# Generate dict (based on what I assume Wordle uses)
#   en-us
#   5-letter words (based on LEN option)
#   no proper nouns (no initial capitals: Tonya, Tokyo, Timex, Texas, etc)
#   no contractions (e.g. "it'll")
#   only ASCII (no accents, e.g. "mêlée")
# =~ 4600 words
# cat /usr/share/dict/american-english |grep '^.....$' |grep -v '^[A-Z]' |grep -v "'" | LANG=C grep '^[a-z]*$' |sort |uniq > english-lower-len-5.dict

dict_file = open(opts.dict)
words_dict = set([w.strip() for w in dict_file])
words_left = words_dict
words_left_n = len(words_left)

# Note, this is case sensitive here, as that's relevant for e.g. German
if opts.target and opts.target not in words_left:
    print(f"Target ({opts.target}) doesn't exist in dictionary ({opts.dict})")
    exit()

if not opts.target and opts.random:
    # To get a random word, seek back to the right pos in file
    i = random.randint(0, words_left_n-1)
    dict_file.seek(i * (LEN+1)) # Fixed line width, plus trailing "\n" byte
    opts.target = dict_file.readline().strip()

dict_file.close()

boost = set()
if opts.boost:
    boost_file = open(opts.boost)
    boost = set([w.strip() for w in boost_file])
    boost_file.close()

################################################################################

# Collect indexes to find words matching certain criteria.
# This DS is a dict[list[set[]]]
# The dict tracks the letters
# The list tracks what position the letter is in (0 for wildcard/yellow position)
# The set tracks the words that meet that criteria.
# candidates['y'][2] contains 'word' # means the letter is the second (1-based) letter in the word

lookup = dict()

for word in words_left:
    # TODO split the rest of this out and just recompute for each round?
    # Then could also more easily just count occurrences of this letter over all letter-positions left
    for pos, letter in enumerate(word):
        lookup[letter] = lookup.get(letter) or [ set() for i in range(LEN+1) ]

        # TODO
        # n_positions_with_letter[letter] =+ 1

        # Note, this word is a candidate for (green, positioned) letter (1-based)
        lookup[letter][pos+1].add(word)

# TODO
# Best way to track which letter frequencies?
# n_words_with_this_letter_at_this_pos / n_words
# n_words_with_this_letter_at_any_pos  / n_words # Union of the words with this letter
# n_letters_that_are_this_letter / n_letters # Over all words, eg (union - intersection) * LEN

# TODO factor this out, and consider re-computing it upon each generation, based on the remaining words?
letter_freq_d = dict()
for letter in lookup:
    s = set()
    letter_freq_d[letter] = [ 0 for i in range(LEN+1) ]
    # 1-based counting of letter positions in word
    for pos in range(1,LEN+1):
        letter_freq_d[letter][pos] = len(lookup[letter][pos]) / words_left_n
        s = s | lookup[letter][pos]

    # What fraction of words have this letter in any pos
    letter_freq_d[letter][0] = len(s) / words_left_n


# This basically implements hard-mode by default, where confirmed letters are subsequently required.
# Keep track of multiples/duplicates required
min_letters = dict()
blacklist_words = set()
guesses_n = 0
guess = ''

while True:

    # Go over remaining candidates
    words_left = set()
    for letter in lookup:
        for pos in range(LEN):
            words_left = words_left | lookup[letter][pos + 1]
            # Really not efficient, but we need to ensure that wildcard letters present:
            for w in lookup[letter][pos + 1]:
                for l in min_letters:
                    if w.count(l) < min_letters[l]:
                        blacklist_words.add(w)

    words_left = words_left - blacklist_words

    guesses_n += 1
    print()
    print(f"Round: {guesses_n:4}")
    print(f"Left:  {len(words_left):4}")
    print()

    if not words_left:
        print('None')
        exit()

    scores = dict()
    for word in words_left:
        # Word freq. Note, this just makes the word more likely to be in the Wordle
        # dictionary, since there's some threshold for excluding less common words.
        # However, given two words in the dictionary, that one is twice as frequent
        # as the other doesn't mean it's more likely to be the target word.

        # from wordfreq import zipf_frequency
        # by_word = zipf_frequency(word, 'en')

        by_word = .1 if word in boost else 0
        # TODO flag words that pass criteria for being target words (stem, no lead cap in EN)

        # Sum of freq of (unique) letters
        by_letter = score_letters(word)

        # A slightly more comparable scale:
        # by_combined = 10 * by_letter + by_word
        by_combined = by_letter + by_word

        scores[word] = {
            'word': word,
            'by_word': by_word,
            'by_letter': by_letter,
            'by_combined': by_combined,
        }

    # Sort for top N objs by key
    scores_sorted = sorted(scores.values(),
                           key=itemgetter('by_combined'),
                           reverse=True)
    if opts.top:
        # Print headings
        print(f"{'lett':>7} {'word':>7} {'combo':>7}")
    for s in scores_sorted[:opts.top]:
        print(
            f"{s['by_letter']:7.4f} {s['by_word']:7.1f} {s['by_combined']:7.4f} {s['word']:20s}"
        )

    print()

    guess = None
    if opts.auto:
        # Was an explicit starting word override given?
        if opts.start:
            scores_sorted.insert(0, {'word': opts.start})
            opts.start = None
        # Auto guess the top-scoring remaining word
        guess = scores_sorted[0]['word']
        print(f"Guess:  {guess}")

    while not guess:
        try:
            guess = input(f"Guess:  ")
        except:
            print()
            exit()

        match = re.match('\s*([+]?)\s*(.*)\s*', guess)
        if match:
            force, guess = match.groups()
        if guess not in words_left and not force:
            # It's a bad guess, because its's already excluded, but allowed, so just warn
            beep()
            # This is not a valid word in the original dictionary
            guess = None

    # Each letter in the reply has a corresponding operator code: exact (+), wild (*), miss (-)
    reply_ops = [None for i in range(opts.length)]

    # eg:  c+a*n-a-l*
    # Which means (0-based index of these chars):
    #   0-1 There's a 'c' at pos 1 (and maybe elsewehere?)
    #   2-3 There's an (or more) 'a' but not at pos 2 (pos 2 out of 5, 1-based)
    #   4-5 There is no 'n'
    #   6-7 There are no *additional* 'a' letters (i.e. just the previously found)
    #   8-9 There's an (or more) 'l', but not at pos 5

    # For display feedback only
    reply = ''

    if opts.target:
        # For tracking duplicate letters in target and guess
        remaining_letters = dict()
        for l in opts.target:
            remaining_letters[l] = remaining_letters.get(l) or 0
            remaining_letters[l] += 1

        # Just looking for exact matches in the first iteration.
        # This is because we need to prioritize scoring exact matches (+) before wilds (*).
        for pos, l in enumerate(guess):
            if l == opts.target[pos]:
                reply_ops[pos] = '+'
                remaining_letters[l] -= 1

        # Now see if there are any duplicate chars left in target for any wild matches (*)
        for pos, l in enumerate(guess):
            if reply_ops[pos]:
                # Already had an exact match
                continue
            elif l in remaining_letters and remaining_letters[l] > 0:
                reply_ops[pos] = '*'
                remaining_letters[l] -= 1
            else:
                reply_ops[pos] = '-'

        reply = ''.join(reply_ops)

    if reply:
        print("Reply: ", reply)
    else:
        while not reply:
            try:
                reply = input("Reply:  ")
            except:
                print()
                exit()

            if len(reply) != LEN or not re.match('^[yon+*_-]+$', reply):
                reply = None
                beep()

    if (opts.target and guess == opts.target) or re.match('^[y+]+$', reply):
        print(f"\nFound: {guesses_n:2d} tries")
        exit()

    # Note if a letter was never seen (and no later positions), as then gray means not present at all
    letter_maybe_del = set()

    # TODO consider if it's more efficient to just maintain a regex and filter out from `remaining` in each iteration?
    # Two masks:
    # filter_in  = [ 's', '.', 'r', '.', '.' ]
    # filter_out = [
    #     [], [a,o,p], [f], [], [t,m]
    # ]
    # TODO
    # The only missing info then is when we know the (min) count of duplicate letters, eg 2+ of 'e'
    # Guess:  semen
    # Reply:  +*_+_
    # Then there may be no remaining candidates that don't have 2+ 'e'

    # Count of required letters, based on previous replies.
    # This assumes hard-mode (that all previous info is used in each subsequent guess)
    min_letters = dict()

    for pos in range(LEN):
        letter = guess[pos]
        op = reply[pos]

        # Note, the below [pos+1] syntax is because 1-based counting in the target word
        if op in 'y+':  # yes
            # Letter is present, at this position.
            min_letters[letter] = min_letters.get(letter) or 0
            min_letters[letter] += 1
            # But maybe also at other positions ... so, don't delete those yet.
            ...
            # However, no *other* letter is at *this* pos, so delete all of those.
            for l in lookup:
                if l != letter:
                    blacklist_words = blacklist_words | lookup[l][pos + 1]
                    lookup[l][pos + 1] = set()
        elif op in 'o*':  # other
            # Letter is still a candidate, but not at this pos.
            # (Might still have (multiple) occurrences of this elsewhere. Don't remove those.)
            blacklist_words = blacklist_words | lookup[letter][pos + 1]
            lookup[letter][pos + 1] = set()
            # This letter is now required at some/any other pos
            min_letters[letter] = min_letters.get(letter) or 0
            min_letters[letter] += 1
        elif op in 'n-_':  # no
            # (The additional '_' is just to also allow to keep the Shift key pressed for all op chars.)
            # Letter is not present in this position:
            blacklist_words = blacklist_words | lookup[letter][pos + 1]
            lookup[letter][pos + 1] = set()

            # Letter *maybe* not present at any other position, but maybe *later* in the word:
            # (Apparently the green letters take priority over the yellow/grey letters).
            letter_maybe_del.add(letter)
            # If letter has no (more) occurrences (including *later* occurrences):
            # Counter-example: target 'shake', but given 's+e-r-v-e+' (clearly not sequential)
            # Because the 'e' in pos 2 is grey, even before the (green) 'e' at pos 5 was processed.

    # After checking each pos for each letter, which were gray once, but otherwise not green later:
    for letter in letter_maybe_del:
        # Did we then find it later as a '*' or '+' letter?
        if letter not in min_letters:
            # Then blacklist all those words with this letter anywhere
            for s in lookup[letter]:
                blacklist_words = blacklist_words | s
            lookup[letter] = [set() for i in range(LEN + 1)]
