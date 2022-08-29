#!/usr/bin/env python
import random
import time
import os

# Debug print the local vars(), also from inside a def
from pprint import pprint
import re
def pvars(_extra:dict=None):
    """Also pass pvars(vars()) from inside a def"""
    _vars = { **globals(), **locals(), **(_extra if _extra else {}) }
    pprint([ [k,_vars[k]] for k in _vars if re.match(r'[a-z]', k)])


def exponential_backoff(callback, max_attempts_n=0, max_sleep_exponent=10):
    # TODO what if callback needs some args ?
    # TODO test harness?
    """
    Demo of a simple exponential backoff

    This simulates attempting some call (eg trying to lock a resource), which in
    this demo fails with a certain probability. This then shows how / how often
    it waits before making another attempt.

    Run multiple instances of this script in the background to see how diff procs behave.
    for i in {1..5}; do ./python/exp_backoff.py& done

    See https://en.wikipedia.org/wiki/Exponential_backoff
    """

    attempted_n = 0

    # Only need the last few digits of pid for this demo:
    pid = '...' + str(os.getpid() % 1000)
    print(f'pid:{pid:>6} n:{attempted_n:>3} START')

    success = None
    while not success:
        if max_attempts_n and attempted_n > max_attempts_n:
            return None

        success = callback()
        if success:
            print(f'pid:{pid:>6} n:{attempted_n:>3} SUCCESS')
            return success

        attempted_n += 1

        # Because randrange() can still be zero (in non-multiplied seconds)
        MIN_SLEEP = .1
        # Units. 1: seconds, 1000: kiloseconds, 1/1000: milliseconds, etc
        multiplier = 1/100

        # Put an upper limit on how much sleep happens, even if we have to retry
        # many attempts. However, if you put a limit on the value passed to
        # sleep(), then you're reducing randomness. So, we rather limit the
        # exponent here.
        sleep_exponent = min(attempted_n, max_sleep_exponent)
        # Note sleep_n is not strictly increasing, but its expectation value is
        sleep_n = MIN_SLEEP + multiplier * random.randrange(2**sleep_exponent)
        # pvars(vars())
        print(f'pid:{pid:>6} n:{attempted_n:>3} sleep {sleep_n:>6.3f} ...')
        time.sleep(sleep_n)


def random_success(prob_success=.1):
    # This demo simulates that call attempts have a random prob_success
    r = random.random()
    # This must not use <= because random() has a half-open interval [0.0, 1.0)
    success = r < prob_success
    return success


if __name__ == "__main__":
    exponential_backoff(random_success)


