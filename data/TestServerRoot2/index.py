import os
import urllib.parse

class Request:
    def __init__(self, environ):
        self.environ = environ
        self.method = environ['REQUEST_METHOD']
        self.query = urllib.parse.parse_qs(environ.get('QUERY_STRING', ''))
        self.body = environ['BODY']

    def __str__(self):
        s = f'Method: {self.method}\r\nQuery: {self.query}\r\nBody: {self.body}'
        return s

class Response:
    def __init__(self, status_code, headers, body):
        self.status_code = status_code
        self.headers = headers
        self.body = body

    def __str__(self):
        headers = ''
        for key, value in self.headers.items():
            headers += f'{key}: {value}\r\n'
        s = f'Status: {self.status_code}\r\n{headers}\r\n{self.body}'
        return s

    def send(self):
        print(str(self), end='')


request = Request(os.environ)


response = Response(
    200,
    {
        'Content-Type': 'text/plain'
    },
    str(request)
)
response.send()
