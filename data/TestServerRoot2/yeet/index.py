#!/usr/bin/env python3

import os

CRLF = "\r\n"
statusLine = "Status: 200"

body = "<html><body><h1>Python 3 CGI Test</h1><p>yeet</p></body></html>"
headers = {
    "Content-Type": "text/html",
    "Content-Length": str(len(body))
}

res = [statusLine,
    *[f"{k}: {v}" for k, v in headers.items()],
    "",
    body
]
response = CRLF.join(res) + CRLF + CRLF

print(response, end="")
