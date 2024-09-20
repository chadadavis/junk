#!/usr/bin/env python3

# TODO move this into dotfiles?

import sys
from rich.console import Console
from rich.markdown import Markdown

if len(sys.argv) > 1:
    file = open(sys.argv[1])
else:
    file = sys.stdin

content = file.read()

# Force ANSI codes, even if output is piped (eg to a pager)
console = Console(force_terminal=True)

try:
    with console.pager(styles=True, links=True):
        console.print(Markdown(content))
except BrokenPipeError:
    # eg just quiting the pager before EOF
    pass

