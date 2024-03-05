#export GATEWAY_INTERFACE="CGI/1.1"
#export SERVER_PROTOCOL="HTTP/1.1"
#export REDIRECT_STATUS="200"
#export REQUEST_METHOD="POST"
#export SCRIPT_FILENAME="/home/kolterdyx/CLionProjects/Webserv/data/TestServerRoot/index.php"
#export REQUEST_URI="/index.php"
#export SERVER_NAME="example.com"
#export CONTENT_TYPE="text/plain"
#export CONTENT_LENGTH="0"

export GATEWAY_INTERFACE=CGI/1.1
export SERVER_PROTOCOL=HTTP/1.1
export REDIRECT_STATUS=200
export REQUEST_METHOD=POST
export SCRIPT_FILENAME=/home/kolterdyx/CLionProjects/Webserv/data/TestServerRoot/index.php
export BODY="Hello world!"
export ACCEPT=*/*
export CONTENT_LENGTH=13
export CONTENT_TYPE=text/plain
export HOST=localhost
export USER_AGENT=curl/7.64.1
export QUERY_STRING="name=Kolterdyx&email=kolterdyx%40gmail.com&message=Hello+world%21"

echo "Hello world!" | /usr/bin/php-cgi

