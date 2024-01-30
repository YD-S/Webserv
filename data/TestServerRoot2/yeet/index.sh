
# Echo raw HTTP response

echo -e 'Status: 200\r'
echo -e 'Content-Type: text/html\r'
echo -e 'Content-Length: 84\r'
echo -e '\r'
echo -e '<html><body><h1>Bash CGI</h1><p>This page was generated using bash</p></body></html>\r'

