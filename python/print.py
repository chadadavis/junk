#!/usr/bin/env python

print("hello world")

# More flexible:
print("place {} and {} and {}".format(1,2,3), '', sep="\n--\n")

# Faster:
print("stuff is %10d units of %-20s things" % (3, 'blah'))

def functions(x: int = 34) -> int:
    print(x)
    return x+1

y = functions(33)
print(y)

desaster = {
    'one': 3,
    "two": 2,
}

print(desaster)

# x = input("gimme: ")
# print("got: " + x)

longin = '''
stuff is like "I wouldn't do that" \
if I wasn't you
'''

print(longin)

print()

x = 2
if x < 3:
    print('x<3')
elif x < 2:
    print('x<2')


x = 'str'
if type(int()) == type(x):
    print('declare ff in a conditional?')
    def ff():
        print('ff')

if x == 2:
    pass

print

for x in 1,2,'ten':
    try:
        print('trying not to fail')
        int(x)
    except Exception as identifier:
        print('exception: ', identifier)
    else:
        print('success')
        if 1 == 2:
            pass
    finally:
        print('finally')

import re

sum(range(1,5))

quote = """
Alright, but apart from the Sanitation, the Medicine, Education, Wine,
Public Order, Irrigation, Roads, the Fresh-Water System,
and Public Health, what have the Romans ever done for us?
"""
 
# Use a for loop and an if statement to print just the capitals in the quote above.

for char in quote:
    print(char, end="") if char.isupper() else ''
print()

listing = ['one','two','three','four','five','six']
for l in [i for i in listing if i != 'three']:
    if l == 'four': continue
    print(l)

def withstate(x, state={}):
    if x in state: return state[x]
    ans = x**2
    state[x] = ans
    print('state is: ', state)
    return ans

for i in range(10):
    withstate(i)

print(
    "line one",
    "line two",
    "Done",
    sep="\n",
)
