#!/usr/bin/env python

# This uses ANSI escape codes directly, not colorama

# 256-color grayscale values range from 232 to 255 (nums: 24/12/6):
for i in range(232, 256, 4):
    print(f"{i:3}\x1b[38;5;{i}m  " + ('█' * 10) + "\x1b[0m")

