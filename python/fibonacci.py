#!/usr/bin/env python


def fibonacci(nth: int) -> int:
    """Get the `n` th (0-based) Fibonacci from [0, 1, 1, 2, 3, 5, 8, 13, 21, ...]"""
    if nth < 0:
        raise(ValueError())
    a = [0, 1]
    for i in range(2, nth+1):
        a.append(a[-1]+a[-2])
        # print(i, a)
    return a[nth]

# print(f'__name__:{__name__}')
if __name__ == "__main__":
    import sys
    print(fibonacci(int(sys.argv[1])))
