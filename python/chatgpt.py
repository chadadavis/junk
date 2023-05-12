#!/usr/bin/env python

import json
import os
import readline
import requests
import sys
import textwrap
from colorama import init, Fore, Style

# Function to make a request to OpenAI's API
# https://github.com/openai/openai-cookbook/blob/main/examples/How_to_format_inputs_to_ChatGPT_models.ipynb

# TODO backlog

# Make Ctrl-L empty the messages and start a fresh conversation, and display that #1 again

messages = []

def get_response(prompt):
    if not prompt: return
    api_key = open(sys.argv[1]).read().rstrip()
    # url = 'https://api.openai.com/v1/engines/davinci/completions'
    url = 'https://api.openai.com/v1/chat/completions'
    messages.append({ 'role': 'user', 'content': prompt })
    data = {
        'messages': messages,
        # 'max_tokens': 50,
        'model': 'gpt-3.5-turbo',
    }
    headers = {
        'Authorization': 'Bearer ' + api_key,
        'Content-Type': 'application/json',
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


# Set terminal title
sys.stdout.write('\x1b]2;' + 'GPT' + '\x07')

# Clear screen
print('\033c')

# Initialize colorama
init()

user_input = ''
if len(sys.argv) > 2:
    user_input = ' '.join(sys.argv[2:])

while True:
    try:
        print("\n" + Fore.YELLOW + str(len(messages)//2+1) + " > ", end='')
        if user_input: print(user_input)
        while not user_input:
            user_input = input()
        if response := get_response(user_input):
            print(Fore.WHITE + "\n" + wrapper(response))
        user_input = None
    except (EOFError):
        # Clear screen
        print('\033c')
        messages = []
    except (KeyboardInterrupt):
        print()
        sys.exit()
