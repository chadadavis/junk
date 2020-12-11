#!/usr/bin/env python
import random
import time

def exp_backoff(prob_success=.1, max_failed_n=10):
    """
    Demo of a simple exponential backoff

    See https://en.wikipedia.org/wiki/Exponential_backoff
    """
    failed_n = 0
    succeeded = False
    while not succeeded:
        r = random.random()
        succeeded = r < prob_success
        if succeeded: break
        failed_n += 1
        # Threshold count of failures, not sleep duration, to keep randomness
        failed_n = max_failed_n if failed_n > max_failed_n else failed_n
        MIN_SLEEP = 1
        # ranrange() can still be zero.
        # sleep is not strictly increasing, but the expectation value is
        sleep_n = MIN_SLEEP + random.randrange(2**failed_n)
        time.sleep(sleep_n)
        # print(f'sleep {sleep_n}')

if __name__ == "__main__":
    exp_backoff()


