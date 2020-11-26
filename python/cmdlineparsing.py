#!/usr/bin/python

import sys
import re
from optparse import OptionParser

# parse command line params
parser = OptionParser()
parser.add_option("-p", "--plot", dest="plot_filename",
				  help="print GNUplot TM prediction for a single protein to FILE",\
				  metavar="FILE")
parser.add_option("-d", "--dist", dest="dist_filename",
				  help="print GNUplot TM distribution for a series of protein to FILE",\
				  metavar="FILE")
parser.add_option("--debug",
				  action="store_false", dest="debug", default=False,
				  help="print debugging information to stdout")

(options, args) = parser.parse_args()
mydict = {1: 1, 2: 2}

print "Options: ", options, " Args: ", args, "\n"


	
if options.plot_filename:
	print "yes"

#if options["plot_filename"]:
#	print options["plot_filename"]


sequence = ""
#s = sys.stdin.readline()
#while s :
#	s = sys.stdin.readline()
#	while s and not re.match('^>', s) :
#		sequence += s
#		s = sys.stdin.readline()
#	print sequence, "\n\n"
#	sequence = ""


something = {}
if not something.has_key(0) :
	something[0] = 1
else:
	something[0] += 1
	
#something[0] += 1
something[1] = 1

print something.keys()


		
	
