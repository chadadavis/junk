#!/usr/bin/env python

def factorial(n):
    if n == 0:
        return 1
    for i in range(n-1, 0, -1):
        n *= i
    return n
f=factorial

if __name__ == "__main__":
    import sys
    i = int(sys.argv[1])
    print(i, f(i))
