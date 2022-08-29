#!/usr/bin/env python

import subprocess

def qx(cmd):
    """Quick capture stdout from an external process, like qx() or backticks in Perl.

    For more explanation, as well as capturing stderr, see this tutorial:
    https://code-maven.com/qx-in-python

    """
    return subprocess.check_output(cmd, shell=True)
