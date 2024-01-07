#!/usr/bin/env python

# Part-of-speech tagging

# https://www.nltk.org/howto/wordnet.html

# 'n': Noun - A person, place, thing, or idea.
# 'v': Verb - An action or state of being.
# 'a': Adjective - A word that describes a noun.
# 's': Adjective Satellite - A special kind of adjective that is always linked to another adjective (the head of a cluster of synonyms).
# 'r': Adverb - A word that modifies a verb, an adjective, or another adverb.

import sys
from nltk.tokenize import word_tokenize
from nltk.corpus import wordnet

# # Ensure you have the required NLTK WordNet data downloaded.
# nltk.download('wordnet')
# : or ; python -m nltk.downloader wordnet

for line in sys.stdin:
    words = word_tokenize(line)
    for word in words :
        # Get all synsets for the word 'mate'
        synsets = wordnet.synsets(word)
        pos_tags = set()
        for synset in synsets:
            pos_tags.add(synset.pos())
        print(word, end='')
        if synsets:
            print('(', end='')
            print(*pos_tags, sep=',', end='')
            print(')', end='')
        print(' ', end='')
    print()

