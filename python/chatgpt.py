import requests
import json
import sys
import readline
import textwrap
import os

messages = []

# Function to make a request to OpenAI's API

def get_response(prompt):
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
        line_wrap = textwrap.wrap(line, WRAP_WIDTH, replace_whitespace=False, drop_whitespace=False)
        line_wrap = line_wrap or ['']
        lines_wrapped += line_wrap
    string = "\t" + "\n\t".join(lines_wrapped)
    return string


# Main loop
print("\nAsk a question:\n")
while True:
    try:
        user_input = input("> ")
        response = get_response(user_input)
        wrapped = wrapper(response)
        print("\n", wrapped, "\n")
    except:
        print()
        exit()
