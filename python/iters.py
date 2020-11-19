#!/usr/bin/env python

class SomeClass:
    def __getitem__(self, pos):
        _items = [4,5,6]
        return _items[pos]

obj = SomeClass()

# Monkey patching a new attribute
obj.field = 3
print(obj.field)

for i in obj:
    print(i)



