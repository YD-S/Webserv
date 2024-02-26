#!/usr/bin/env python3

# read stdin
import sys
import os
content_length = 0
if "HTTP_CONTENT_LENGTH" in os.environ:
    content_length = int(os.environ["HTTP_CONTENT_LENGTH"])
    if content_length > 0:
        req_body = sys.stdin.read(content_length)
    else:
        req_body = "Request body is empty"
else:
    req_body = "No request body"

CRLF = "\r\n"
statusLine = "Status: 200"

body = f"<html><body><h1>Python 3 CGI Test</h1><p>The content passed in the request body is:</p><pre>{req_body}</pre><ul><li>{content_length}</li></ul></body></html>"
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
