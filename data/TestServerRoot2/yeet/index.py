#!/usr/bin/env python3

import os

response = ""

def p(*args, **kwargs):
    global response
    response += " ".join(args) + "\r\n"

status = "200 OK"

body = "<html><body><h1>Python 3 CGI Test</h1><p>yeet</p></body></html>"
headers = {
    "Content-type": "text/html",
    "Content-length": str(len(body))
}

p(f"HTTP/1.1 {status}")
for k, v in headers.items():
    p(f"{k}: {v}")
p()
p(body)
p()
with open(f"{os.path.dirname(__file__)}/output.txt", "w") as f:
    f.write(response)

print(response, end="")