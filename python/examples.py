#!/usr/bin/python

import sys
import os

def func():
	"""
	Documention for some function. First line contains short desc.

	Remaining lines contain long desc.
	It does cool stuff.
	"""
	
	print "Hello" + " it works"
	
# end func

print "Loading ... " + __name__
print "dir()", dir()

if "1" == 1 and "2" == 2 :
	print "Automatic string conversion exists."
elif 1 == 2:
	print "Bad stuff ..."
else:
	print "No Automatic string conversion."

print "Current dir ..."
os.system("/bin/ls")

############################################
# strings

x = "hübsch"
print("print(x)")
print(x)
print("print(x[0])")
print(x[0])
print("print(x[2:4])")
print(x[2:4])
print("print(x[2:])")
print(x[2:])
print("print(x[:2])")
print(x[:2])
print("print(x[-2:])")
print(x[-2:])
print("print(x[:-2])")
print(x[:-2])
print # prints a blank line

# illegal
#x[2]='x'

############################################
# lists

l = [2, 3]
print(len(l))
l.append(4)
print(l)

y = 0
for x in l:
	print y, x # if parens () included in command, they're printed !
#	print(y, x) # prints (0, 2) for example
	y += 1

for x in range(10):
	print x,
print

# multiple assignment (left to right)
# (Fibonacci series)
print "Fibonaccis:"
a, b = 0, 1
while b < 10:
	print(b), # trailing comma, prevents new-line after a 'print'
	a, b = b, a + b
print

def is_odd(x):
	"""
	returns true if x is odd
	"""
	if x % 2: return 1
	else: return 0

print "Filtering odd numbers"
# returns the members of list for which function is true
print filter(is_odd, range(10))

print "mapping"
# calls is_odd on each member of range(10), returns a list
print map(is_odd, range(10))

print "reducing"
# calls function on contents of range() two at a time
print reduce(lambda x, y: x + y, range(5))


# list comprehension
print "comprehension"
print [x for x in range(10) if x % 2]

print [[x, x**2] for x in range(10)]
# creates/prints a multiplication table
print [[x, y, x*y] for x in range(1,5) for y in range(1,5)]

print "deletion"
x = 4
del x

x = [1, 2, 4, 8]
del x[1:3]
print x
del x

print "dictionaries"
dict = {'one' : 'eins', 'two' : 'zwei', 'three' : 'drei'}
print dict['one']
del dict['one']
print dict
print dict.keys()
if dict.has_key('two') and dict.has_key('three'): print "two/three exists"

print "new dir()", dir()
# items are defined as file is read
# dir() prints are defined names up to that point

print "alt print"
sys.stdout.write("hello\n")

x = "a string with the number " + `5` + " in it"
print x

if not "": print "String is empty"

print "Input testing"
#while 1:
#	try:
#		s = raw_input("Enter some value: ")
#		break
#	except:
#		print
#print s
#
#x = int(raw_input("Enter an integer: "))
#print x + 5

#arr = []
#for i in range(5):
#	while 1:
# 		try:
#			arr.append(int(raw_input("Enter integer " + str(i) + ": ")))
#			break
# 		except:
# 			print
#
#print "arr: ", arr
#arr.sort()
#print "smallest is: ", arr[0]

def get_int():
	while 1:
		try:
			n = int(raw_input("Enter n: "))
			break
		except:
			print
	return n

def fib():
	print "Compute the nth fibonacci number: "
	n = get_int()
	a, b = 0, 1
	for i in range(n):
		a, b = b, a + b
	print b

fib()

					   
		
