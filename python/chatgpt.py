#!/usr/bin/env python

import argparse
import json
import os
import readline
import sys
import textwrap

import requests
from colorama import Fore, Style, init

# Function to make a request to OpenAI's API
# https://github.com/openai/openai-python
# https://github.com/openai/openai-cookbook/blob/main/examples/How_to_format_inputs_to_ChatGPT_models.ipynb

# TODO backlog

# Read from STDIN and append to prompt

# Parse out any files from sys.argv and upload/append them to the prompt.

messages = []

def get_response(prompt, key, model):
    if not prompt: return
    # url = 'https://api.openai.com/v1/engines/davinci/completions'
    url = 'https://api.openai.com/v1/chat/completions'
    headers = {
        'Authorization': 'Bearer ' + key,
        'Content-Type': 'application/json',
    }
    messages.append({ 'role': 'user', 'content': prompt })
    data = {
        # 'max_tokens': 50,
        'model': model,
        'messages': messages,
    }
    response = requests.post(url, data=json.dumps(data), headers=headers)
    response_json = response.json()
    if 'error' in response_json:
        print(response_json['error'])
        return
    # print(response_json)
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


parser = argparse.ArgumentParser()
parser.add_argument('-i', '--interactive', action='store_true', help="Continue the conversation after the first response")
parser.add_argument('-m', '--model',       type=str,            help="Name of OpenAI model, eg gpt-4, default gpt-3.5-turbo", default='gpt-3.5-turbo')
parser.add_argument('-k', '--keyfile',     type=str,            help="Path to file containing your OpenAI API key")
parser.add_argument('rest', nargs=argparse.REMAINDER)
args = parser.parse_args()
key = open(args.keyfile).read().rstrip()

# Interactive mode if no CLI params given, or force it with -i param
args.interactive = len(args.rest) == 0 or args.interactive

user_input = ' '.join(args.rest)

if not args.interactive:
    # Just print the response, unformatted, and exit
    print(get_response(user_input, key=key, model=args.model))
    sys.exit()

# Interactive mode:

# Set terminal title
sys.stdout.write('\x1b]2;' + 'GPT' + '\x07')

# Clear screen
print('\033c')

# Initialize colorama
init()

while True:
    try:
        print("\n" + Fore.YELLOW + str(len(messages)//2+1) + " > ", end='')
        if user_input: print(user_input)
        while not user_input:
            user_input = input()
        if response := get_response(user_input, key=key, model=args.model):
            print(Fore.WHITE + "\n" + wrapper(response))
        user_input = None
    except (EOFError):
        # Clear screen
        print('\033c')
        messages = []
    except (KeyboardInterrupt):
        print()
        sys.exit()
