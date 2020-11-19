#!/usr/bin/env python

choices = [
    'quit',
    'spam',
    'eggs',
    'smack down',
]

def print_menu():
    print()
    for i, item in enumerate(choices):
        print(f'{i}\t{item}')

cart = {}

while True:
    print(f"Cart: {cart}")
    print_menu()
    try:
        opt = int(input("Choice: "))
        assert 0 <= opt < len(choices)
        choice = choices[opt]
    except:
        continue
    if opt:
        print(f"Adding: {choice} (#{opt})")
        if choice not in cart:
            cart[choice] = 1
        else:
            # Assume toggle mode
            del cart[choice]
    else:
        print("Quitting")
        break