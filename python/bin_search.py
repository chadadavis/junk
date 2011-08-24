#!/usr/bin/python

import math

debug = 1

def debug_print(*args):
	if debug: print(args)

def bin_search(key, list):
	"""
	Binary search for 'key' in 'list'
	"""
	
	min = 0
	max = len(list)
	mid = (min + max) / 2

	# This algo runs for log(x) iterations. Logarithm to base 2
	for i in range(1 + math.log(max) / math.log(2)):
		debug_print("%2d: min: %2d mid: %2d max: %2d" % (i, min, mid, max))
		
		if list[mid] == key:
			return mid
		elif list[mid] < key:
			min = mid
			mid = (min + max) / 2
		else:
			max = mid
			mid = (min + max) / 2

	return -1

################################################################################
# Test code

# a = [2,3,4,5,5,8,9,11,13,14,16,17,17,19,22,27,33,34,36,44,66,70,70,71]
# b = [11, 13, 22, 33, 34, 44, 66]
# 
# for i in b:
# 	print "searching for ", i
# 	print bin_search(i, a)
	

	
