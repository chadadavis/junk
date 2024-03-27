#!/usr/bin/env python

# Alternatives to this script:
# https://github.com/kharvd/gpt-cli
# https://github.com/0xacx/chatGPT-shell-cli


# OpenAI's API
# https://openai.com/pricing
# https://platform.openai.com/docs/api-reference/chat/
# https://github.com/openai/openai-python
# https://github.com/openai/openai-cookbook/blob/main/examples/How_to_format_inputs_to_ChatGPT_models.ipynb

# TODO backlog

# Put this into its own repo (so that vscode uses just one venv per workspace/repo)

# And /file to add/list attached files (text for now) (with readline completion of filenames)

# history: give each thread a title/date-stamp/hash (and separate files in ~/.config/chatgpt/...)
# And /history to list/resume a previous history session/thread (readline completion)
# And /clear to clear the conversation/thread/start anew
# And ask GPT (as 'system' user) to provide short (< 50 chars) summary of topic/question/conclusion for each.

# But note that readline is only logging user messages, not assistant replies
# Re-format them, so keep track of which 'role' was behind each message
# Default to --resume prev session if new enough. Default to --new if the last is too old.
# In between? Then prompt to resume (Default: new/don't resume)
# If logging to separate files, then also rotate the logs (can logrotate due that automatically?)
# The Assistants API allows for conversation threads to have an ID (managed server-side) that I could resume:
# https://platform.openai.com/docs/assistants/overview
# Keep a week/month/quarter or so of logs?

# Allow /commands from inside the conversation
# And ability to list all the commands (and docs)
# Also a parser/regex for each command and its args

# And /model to change the model, and complete possibilities (autocomplete?)
# And /stats to show usage/quota/spend
# And /instructions to edit custom instructions (external EDITOR)
# And /shell to run a shell command (or prefix with !)
# (But allow the assistant to see that output, so I can ask questions about it)
# Consider also echo'ing the bash session, eg via 'script' or similar, so that I can ask questions about the output of commands?
# But then we'll also want to be wary of PII
# https://platform.openai.com/docs/assistants/overview

# Readline:
# complete words from the conversation history?
# Do I need to manually keep my own dict of (long) words (without punctuation) ?

# use https://pypi.org/project/rich/ for formatting, etc
# eg Alternate background color (subtle) between user/assistant

# Consider using the streaming API ? (To get incremental output/typing, like the web version)
# https://cookbook.openai.com/examples/how_to_stream_completions

################################################################################

import argparse
import json
import os
import pprint
import readline as rl
import select
import subprocess
import sys
import textwrap
from typing import Optional

import colorama
import regex
import requests
from colorama import Back, Fore, Style

pp = pprint.PrettyPrinter().pprint


def completer(text: str, state: int) -> Optional[str]:
    completions = []
    if not text:
        return None

    # print(f'\ntext:{text}:', file=sys.stderr)
    # Completions via (current) history session
    # TODO actually better to maintain a dict from the messages?
    for i in range(1, rl.get_current_history_length() + 1):
        i = rl.get_history_item(i)
        # TODO tokenize / remove punctuation from i ?
        if i.casefold().startswith(text.casefold()):
            completions += [ i ]

    # Complete user /commands
    # NB, make sure that completer_delims doesn't contain '/'
    if text.startswith('/'):
        completions += [
            '/' + cmd for cmd in commands if cmd.casefold().startswith(text[1:].casefold())
        ]

    # Complete file names matching ${text}*
    if '/' in text:
        dir = os.path.dirname(text)
        if dir != '/': dir += '/'
        bn = os.path.basename(text)
        # print(f'\n{dir=}')
        # print(f'\n{bn=}')
        for file in os.listdir(dir):
            if not bn or file.startswith(bn):
                if os.path.isdir(dir + file): file += '/'
                # print(f'\n{file=}')
                completions.append(dir + file)

    if state < len(completions):
        return completions[state]

    if state == 0:
        # text doesn't match any possible completion
        beep()

    return None


def beep(n: int = 2):
    for _ in range(n):
        print("\a", end='', flush=True)


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


INDENT = 0
def width():
    LINE_WIDTH = os.get_terminal_size().columns
    WRAP_WIDTH = min(80, int(LINE_WIDTH * .9))
    return WRAP_WIDTH


def wrapper(string):
    WRAP_WIDTH = width()

    lines_wrapped = []
    for line in string.splitlines():
        line_wrap = textwrap.wrap(line, WRAP_WIDTH, replace_whitespace=False, drop_whitespace=True)
        line_wrap = line_wrap or ['']
        lines_wrapped += line_wrap
    indent = ' ' * INDENT
    string = indent  + ('\n' + indent).join(lines_wrapped)
    return string


def hr():
    WRAP_WIDTH = width()
    print(Fore.LIGHTBLACK_EX + '\r' + '─' * WRAP_WIDTH, end='\n')


def editor(content_a: str='', /) -> str:
    """Edit a (multi-line) string, by running your $EDITOR on a temp file

    TODO module
    """

    tf_name = '/tmp/' + os.path.basename(__file__).removesuffix('.py') + '.tmp'
    with open(tf_name, 'w') as tf:
        tf.write(content_a)

    # The split() is necessary because $EDITOR might contain multiple words
    subprocess.call(os.getenv('EDITOR', 'nano').split() + [tf_name])

    with open(tf_name, 'r') as tf:
        content_b = tf.read()
    os.unlink(tf_name)
    return content_b


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
    help="Continue the conversation after the first response (not compat. with piped input)",
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
    help="Name of OpenAI model, eg: gpt-3.5-turbo (default), gpt-4 , gpt-4-turbo-preview , ",
    default='gpt-3.5-turbo',
)
parser.add_argument(
    '--history',
    type=str,
    help="Path to history file, default ~/.config/chatgpt/history.txt",
    default='~/.config/chatgpt/history.txt',
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

# User /commands
# TODO also dispatch to the methods that process the args
commands = {}
commands['model'] = {
    'desc': 'Get/set the OpenAI model',
    'usage': '/model [model-name]',
    'example': '/model gpt-4-turbo-preview',
}
commands['revert'] = {
    'desc': 'Revert/remove the previous user message (and assistant reply)',
    'usage': '/revert',
    'example': '/revert',
}
commands['edit'] = {
    'desc': 'Edit the last user message in external $EDITOR',
    'usage': '/edit',
    'example': '/edit',
}
commands['file'] = None

# History of all user/assistant messages in this conversation dialog
messages = []


################################################################################
# TODO put this in a main function

# Initialize colorama
colorama.init(autoreset=True)

# Init readline completion
rl.set_completer(completer)
rl.set_completer_delims(' ;?!*"\'') # NB, avoid . and / to complete file paths
rl.parse_and_bind("tab: complete")

# Init readline history
args.history = os.path.expanduser(args.history)
hist_dir = os.path.dirname(args.history)
os.makedirs(hist_dir, exist_ok=True)
if not os.path.isfile(args.history):
    open(args.history, 'a').close()
rl.read_history_file(args.history)

# This should allow the user to arbitrarily expand long-words from their previous history
# TODO Why is this not binding/working ?
rl.parse_and_bind(r'"\e/":dabbrev-expand')

# Enable bracketed paste mode (allows pasting multi-line content into the prompt)
os.system('printf "\e[?2004h"')
# Disable bracketed paste mode
# os.system('printf "\e[?2004l"')

# Check if the keyfile exists and is readable
# TODO any way to verify it's a usable key (eg with a test request) ?
if not os.path.isfile(args.keyfile):
    print('\n' + Fore.RED + "Error: Cannot read OpenAI API key file: " + Fore.WHITE + Style.BRIGHT + args.keyfile + '\n')
    parser.print_help()
    exit(1)
key = open(args.keyfile).read().rstrip()

# Load any custom instructions
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

# The goal of interactive mode is to allow for follow-up questions.
# If a question was given on the CLI, assume no follow-up, unless -i given
args.interactive = len(args.rest) == 0 or args.interactive
# Any question/prompt written directly on the CLI
init_input = ' '.join(args.rest)

if args.debug: pp(args)

# Check if there is any data (already) piped into stdin, if so delimit it
if select.select([sys.stdin,],[],[],0.0)[0]:
    init_input += '\n```\n'
    init_input += sys.stdin.read()
    init_input += '\n```\n'
    print('\n', init_input, '\n')
    # Interactive mode reads from stdin, so it's not compat with piped input
    args.interactive = False

if not args.interactive:
    # Just print the response, unformatted, and exit
    if response := get_response(init_input, key=key, model=args.model):
        print(wrapper(response))
    sys.exit()


################################################################################
# Interactive mode:
# TODO factor this out

# NB, I tried to keep interactive mode in the case of piped input, but I failed.
# Since the piped input is on stdin, once it's consumed, it's stuck in EOF, it seems.
# But the interactive mode reads from stdin, via input(). So, these modes conflict.
# I tried to re-open stdin from /dev/tty, but that seems to break readline.
# Rather: use multi-line paste from within interactive mode, or use /edit, or use /file

# Multi-line typing?
# And how to make Shift-Enter or Alt-Enter insert a newline, rather than ending the input() ?
# And how to make Ctrl-Enter send the message, rather than ending the input() ?

# Set terminal title
sys.stdout.write('\x1b]2;' + 'gpt-cli' + '\x07')

# Clear/scroll screen
# print('\033c')
print()

# (User) message counter width
DIGITS = 2

while True:
    try:
        # Counts the number of user messages (since they always alternate?)
        i = len(messages) // 2 + 1
        prompt_items = [ *[]
            , '▼ '
            # , ' ' * min(2,INDENT-DIGITS-1)
            , f'{i:{DIGITS}d}'
            # , '\n'
        ]
        prompt = ''.join(prompt_items)
        if init_input:
            user_input = init_input
            init_input = None
        else:
            user_input = None
        if user_input:
            print(Style.DIM + prompt)
            print(user_input)
        while not user_input:
            # NB, no non-printing chars/formatting codes in the input prompt.
            # Else readline miscalculates line length, which breaks editing.
            print(Style.DIM + prompt)
            user_input = input(' ' * INDENT)

        hist_len = rl.get_current_history_length()

        if match := regex.match(r'^\/model\s*([a-z0-9.-]+)?\s*$', user_input):
            # /meta commands
            if match.group(1): args.model = match.group(1)
            print(Fore.LIGHTBLACK_EX + f"model={args.model}")
            user_input = None
        elif match := regex.match(r'^\/edit\s*$', user_input):
            print(f'Editing ... ', end='', flush=True)
            # Get the last input, before the /edit command
            prev = rl.get_history_item(hist_len-1)
            rl.remove_history_item(hist_len-1) # Remove the /edit command
            user_input = editor(prev)
            print(Style.DIM + '\r\n' + user_input)
            if input(Style.BRIGHT + "Submit? (Y/n): ").casefold() == 'n':
                rl.remove_history_item(hist_len-1)
                continue
            rl.add_history(user_input)
        elif match := regex.match(r'^\/revert\s*$', user_input):
            prev = rl.get_history_item(hist_len-1)
            rl.remove_history_item(hist_len-1) # Remove the / command
            rl.remove_history_item(hist_len-2) # Remove the prev content
            print(Style.DIM + '\r\n' + 'Removed: ' + prev)
            # TODO verify that these correspond
            if messages: messages.pop() # Remove the assistant response
            if messages: messages.pop() # Remove the user prompt
            user_input = None
        elif match := regex.match(r'^\/(.*?)\s*$', user_input):
            print("Unknown command: " + match.group())
            continue

        # Do this in every iteration, since we could abort any time
        rl.write_history_file(args.history)

        if user_input:
            print('... ', end='')
            # TODO allow this to be Ctrl-C interrupted
            if response := get_response(user_input, key=key, model=args.model):
                hr()
                print(Fore.WHITE + '\r\n' + wrapper(response) + '\n')

        user_input = None
    except (KeyboardInterrupt):
        # Just cancel/reset the current line/prompt
        print('^C')
    except (KeyboardInterrupt, EOFError):
        # Ctrl-D to exit
        print()
        sys.exit()
