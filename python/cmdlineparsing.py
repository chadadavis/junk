#!/usr/bin/python

import sys
import re
import argparse

# parse command line params
parser = argparse.ArgumentParser()
parser.add_argument(
    "-p",
    "--plot",
    dest="plot_filename",
    help="print GNUplot TM prediction for a single protein to FILE",
    metavar="FILE",
)
parser.add_argument(
    "-d",
    "--dist",
    dest="dist_filename",
    help="print GNUplot TM distribution for a series of protein to FILE",
    metavar="FILE",
)
parser.add_argument(
    "--debug",
    action="store_true",
    # dest="debug",
    # default=False,
    help="print debugging information to stdout",
)

args = parser.parse_args()
my_dict = {1: 1, 2: 2}

print(" Args: ", args, "\n")

if args.plot_filename:
    print("yes")

sequence = ""
#s = sys.stdin.readline()
#while s :
#    s = sys.stdin.readline()
#    while s and not re.match('^>', s) :
#        sequence += s
#        s = sys.stdin.readline()
#    print sequence, "\n\n"
#    sequence = ""


something = {}
if not 0 in something :
    something[0] = 1
else:
    something[0] += 1

# Raises KeyError if key 0 doesn't already exist
#something[0] += 1
something[1] = 1
print(something.keys())
