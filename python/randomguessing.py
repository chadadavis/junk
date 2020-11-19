import random
try:
    limit = int(input('limit: '))
    assert(limit > 0)
except:
    limit = 10
r = random.randint(1, limit)
guess = None
while r != guess:
    try:
        guess = int(input("Guess (1:{}): ".format(limit)))
    except:
        guess = None
    if not guess:
        break
    if r == guess:
        print('nailed it')
    if r > guess:
        print('higher')
    if r < guess:
        print('lower')

print('bye')