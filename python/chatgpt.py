#!/usr/bin/env python

import argparse
import json
import os
import pprint
import readline
import select
import sys
import textwrap

import colorama
import requests
from colorama import Fore, Style

# Function to make a request to OpenAI's API
# https://openai.com/pricing
# https://platform.openai.com/docs/api-reference/chat/
# https://github.com/openai/openai-python
# https://github.com/openai/openai-cookbook/blob/main/examples/How_to_format_inputs_to_ChatGPT_models.ipynb

# TODO backlog
def backlog():
    ...

# Switch from the low-level `select` module to higher-level `selectors` module, if possible.

# BUG the select() call prevents any subsequent input() calls from working correctly.
# Why? How to reset the stdin ?
# This means that the --interactive mode no longer works if you're also piping data into stdin
# (But that would be useful, if you want to pipe some data in and then ask followup questions about it)
# Would the `selectors` module help?
# https://docs.python.org/3/library/selectors.html
# Or what about opening /dev/stdin as a file (given as a CLI arg)?

# Add (multiple?) --file args and upload/append them to the prompt/input

# Consider logging the messages/session to a (timestamped) file, so that I can resume a previous session?
# Default to --resume prev session is new enough. Default to --new if the last is too old. In between? Then prompt to resume (Default: new/don't resume)
# If logging everything, then also rotate the logs (can logrotate due that automatically?)
# The Assistants API allows for conversation threads to have an ID (managed server-side) that I could resume:
# https://platform.openai.com/docs/assistants/overview

# Consider also echo'ing the bash session, eg via 'script' or similar, so that I can ask questions about the output of commands?
# But then we'll also want to be wary of PII
# Or consider using the Assistants API to keep track of a thread server-side
# https://platform.openai.com/docs/assistants/overview

# Consider using the streaming API ? (To get incremental output/typing, like the web version)
# https://cookbook.openai.com/examples/how_to_stream_completions

################################################################################

# History of all messages in this conversation dialog
messages = []


def get_response(prompt, key, model):
    if not prompt: return
    url = 'https://api.openai.com/v1/chat/completions'
    headers = {
        'Authorization': 'Bearer ' + key,
        'Content-Type': 'application/json',
    }
    messages.append({ 'role': 'user', 'content': prompt })
    data = {
        # 'max_tokens': 50,
        'temperature': 0,
        'model': model,
        'messages': messages,
    }
    if args.debug : pp(data)
    response = requests.post(url, data=json.dumps(data), headers=headers)
    response_json = response.json()
    if 'error' in response_json:
        print(response_json['error'])
        return
    if args.debug : pp(response_json)
    content = response_json['choices'][0]['message']['content']
    messages.append({ 'role': 'assistant', 'content': content })
    return content


def wrapper(string):
    LINE_WIDTH = os.get_terminal_size().columns
    WRAP_WIDTH = int(LINE_WIDTH * .8)

    lines_wrapped = []
    for line in string.splitlines():
        line_wrap = textwrap.wrap(line, WRAP_WIDTH, replace_whitespace=False, drop_whitespace=True)
        line_wrap = line_wrap or ['']
        lines_wrapped += line_wrap
    string = "  "  + "\n  ".join(lines_wrapped)
    return string


pp = pprint.PrettyPrinter().pprint

# Initialize colorama
colorama.init(autoreset=True)

parser = argparse.ArgumentParser()
parser.add_argument(
    '-k',
    '--keyfile',
    type=str,
    help="Path to file containing your OpenAI API key, default: ~/.config/chatgpt/api-key.txt",
    default=os.path.join(os.environ['HOME'], '.config/chatgpt/api-key.txt'),
)
parser.add_argument(
    '-i',
    '--interactive',
    action='store_true',
    help="Continue the conversation after the first response",
)
parser.add_argument(
    '-f',
    '--file',
    type=str,
    action='append', # Collect into a list
    default=[],
    help="Path to a file to upload/analyze. Accepts multiple --file args.",
)
parser.add_argument(
    '--instructions',
    type=str,
    help="Path to file containing custom instructions, default: ~/.config/chatgpt/custom-instructions.txt",
    default=os.path.join(os.environ['HOME'], '.config/chatgpt/custom-instructions.txt'),
)
parser.add_argument(
    '-m',
    '--model',
    type=str,
    help="Name of OpenAI model, eg: gpt-3.5-turbo (default), gpt-4, gpt-4-turbo-preview ",
    default='gpt-3.5-turbo',
)
parser.add_argument(
    '-d',
    '--debug',
    action='store_true',
)
parser.add_argument(
    'rest',
    # Suck up remaining CLI args into `rest`. This is the first input/prompt.
    nargs=argparse.REMAINDER,
)

args = parser.parse_args()

if not os.path.isfile(args.keyfile):
    print('\n' + Fore.RED + "Error: Cannot read API key file: " + Fore.WHITE + Style.BRIGHT + args.keyfile + '\n')
    parser.print_help()
    exit(1)
key = open(args.keyfile).read().rstrip()

if os.path.isfile(args.instructions):
    if args.debug :
        info = '\n' + 'INFO: Custom instructions: file://' + Style.BRIGHT + args.instructions + '\n'
        print(info, file=sys.stderr)
    with open(args.instructions, 'r') as file:
        instructions = file.read()
        messages.append({ 'role': 'system', 'content': instructions })


# Prepend (the content of) any file(s) that the question/prompt might reference.
for file_name in args.file:
    if args.debug :
        info = '\n' + 'INFO: File to analyze: ' + Style.BRIGHT + file_name + '\n'
        print(info, file=sys.stderr)
    with open(file_name, 'r') as file:
        messages.append({
            'role': 'system',
            'content': f"Context file: {file_name} (Make use of it when answering subsequent questions)",
        })
        messages.append({ 'role': 'user', 'content': file.read() })


# Interactive/conversation/dialog mode if no CLI params given, or force it with -i param
args.interactive = len(args.rest) == 0 or args.interactive

if args.debug: pp(args)

# Any question/prompt written directly on the CLI
user_input = ' '.join(args.rest)

# Check if there is any data (already) being piped into stdin, if so delimit it
if select.select([sys.stdin,],[],[],0.0)[0]:
    user_input += '\n\nstdin:\n```\n'
    for line in sys.stdin:
        user_input += line
    user_input += '\n```\n'

if user_input:
    print()
    print(user_input)
    print()

if not args.interactive:
    # Just print the response, unformatted, and exit
    if response := get_response(user_input, key=key, model=args.model):
        print(wrapper(response))
    sys.exit()

# Interactive mode:

# And re-open stdin to read any subsequent user/keyboard input()
# This is necessary in case stdin was piped in, which would be stuck on EOF now.
sys.stdin = open('/dev/tty')

# Set terminal title
sys.stdout.write('\x1b]2;' + 'GPT' + '\x07')

# Clear screen
# print('\033c')

while True:
    try:
        print("\n" + Fore.YELLOW + str(len(messages)//2+1) + " > ", end='')
        if user_input:
            print(user_input)
        while not user_input:
            user_input = input()
        if response := get_response(user_input, key=key, model=args.model):
            print(Fore.WHITE + "\n" + wrapper(response))
        user_input = None
    except (KeyboardInterrupt, EOFError):
        print()
        sys.exit()
