#!/usr/bin/env /home/chdavis/git/junk/python/venv/bin/python

import sys
from rich.console import Console
from rich.markdown import Markdown

if len(sys.argv) > 1:
    file = open(sys.argv[1])
else:
    file = sys.stdin

content = file.read()
console = Console()
console.print(Markdown(content))
