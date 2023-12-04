
# Bash CGI script to display the contents of the current directory

if [ "$REQUEST_METHOD" != "GET" ]; then
  echo "Status: 405 Method Not Allowed"
  echo "Allow: GET"
  echo "Content-type: text/plain"
  echo ""
  echo "This script only supports GET requests."
  exit 1
fi

echo "Content-type: text/html"
#echo "Status: 201 No Content"
echo ""
echo "<html><head><title>Index of /</title></head><body>"
echo "<h1>Index of /</h1><hr><pre>"
ls -l
echo "</pre><hr></body></html>"
