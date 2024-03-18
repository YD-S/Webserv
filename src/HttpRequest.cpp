//
// Created by kolterdyx on 14/09/23.
//

#include "HttpRequest.hpp"


HttpRequest::HttpRequest() {
    this->_version = "HTTP/1.1";
}

HttpRequest::HttpRequest(const HttpRequest &other) {
    *this = other;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &other) {
    if (this != &other) {
        this->_method = other._method;
        this->_path = other._path;
        this->_version = other._version;
        this->_headers = other._headers;
        this->_params = other._params;
        this->_body = other._body;
    }
    return *this;
}

HttpRequest::~HttpRequest() {
}

HttpRequest *HttpRequest::setMethod(const std::string &method) {
    this->_method = method;
    return this;
}

HttpRequest *HttpRequest::setPath(const std::string &path) {
    this->_path = path;
    return this;
}

HttpRequest *HttpRequest::setVersion(const std::string &version) {
    this->_version = version;
    return this;
}

HttpRequest *HttpRequest::setHeader(const std::string &key, const std::string &value) {
    _headers.insert(std::make_pair(stringToLower(key), value));
    return this;
}

const HttpRequest *HttpRequest::setBody(const std::string &_body) {
    this->_body = _body;
    return this;
}

const std::string &HttpRequest::getMethod() const {
    return _method;
}

const std::string &HttpRequest::getPath() const {
    return _path;
}

const std::string &HttpRequest::getVersion() const {
    return _version;
}


const std::map<std::string, std::string> &HttpRequest::getHeaders() const {
    return _headers;
}

std::string HttpRequest::getBody() const {
    return _body;
}

std::string HttpRequest::toRawString() const {
    std::string rawString = _method + " " + _path;
    // Add the query parameters to the raw string
    if (!_params.empty())
        rawString += "?" + getQueryString();
    rawString += " " + _version + "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
        rawString += it->first + ": " + it->second + "\r\n";
    }
    rawString += "\r\n";
    rawString += _body;
    return rawString;
}

std::string HttpRequest::toPrintableString() const {
    std::string request = this->toRawString();
    // replace \r\n with \n
    std::string::size_type pos = 0;
    while ((pos = request.find("\r\n", pos)) != std::string::npos) {
        request.replace(pos, 2, "\n");
        pos += 1;
    }
    return request;
}

const std::map<std::string, std::string> &HttpRequest::getParams() const {
    return _params;
}

HttpRequest *HttpRequest::addParam(const std::string &key, const std::string &value) {
    _params.insert(std::make_pair(key, value));
    return this;
}

std::string HttpRequest::getHeader(const std::string &key) const {
    if (_headers.find(stringToLower(key)) == _headers.end()) {
        LOG_ERROR("Header " << key << " not found");
        return "";
    }
    return _headers.find(stringToLower(key))->second;
}

std::string HttpRequest::getParam(const std::string &key) const {
    return _params.find(key)->second;
}

std::string HttpRequest::getQueryString() const {
    std::string queryString = "";
    for (std::map<std::string, std::string>::const_iterator it = _params.begin(); it != _params.end(); ++it) {
        queryString += it->first + "=" + it->second + (it != _params.end() ? "&" : "");
    }
    queryString = queryString.substr(0, queryString.size() - 1);
    return queryString;
}

std::string HttpRequest::trim(const std::string &str) {
    std::size_t first = str.find_first_not_of(" \t\r");
    if (first == std::string::npos)
        return "";

    std::size_t last = str.find_last_not_of(" \t\r");
    return str.substr(first, last - first + 1);
}

void HttpRequest::parseParams(const std::string &path_string) {

    // Find the position of the '?' character

    size_t query_pos = path_string.find('?');

    // Check if there are any parameters
    if (query_pos != std::string::npos) {
        // Extract the query string
        std::string query = path_string.substr(query_pos + 1);

        _path = _path.substr(0, query_pos);
        // Use std::istringstream to tokenize the query string
        std::istringstream iss(query);
        std::string param;

        while (std::getline(iss, param, '&')) {
            // Split each parameter into key and value
            size_t equal_pos = param.find('=');
            if (equal_pos != std::string::npos) {
                std::string key = param.substr(0, equal_pos);
                std::string value = param.substr(equal_pos + 1);
                _params[key] = value;
            }
        }
    }
}

void HttpRequest::printHttpRequest() const {
    std::cout << "Method: " << _method << std::endl;
    std::cout << "Path: " << _path << std::endl;
    std::cout << "HTTP Version: " << _version << std::endl;

    // Print parameters
    std::cout << "Parameters:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _params.begin(); it != _params.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    // Print headers
    std::cout << "Headers:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }

    // Print body
    //std::cout << "\nBody:\n\n" << _body << std::endl;

}

int HttpRequest::isInvalid(const LocationConfig *location) const{
    if (_method != "GET" && _method != "POST" && _method != "DELETE")
        return 405;
    if (_path.empty() || _path[0] != '/')
        return 400;
    if (_version != "HTTP/1.1")
        return 505;
    if (_body.size() > location->getClientMaxBodySize())
        return 413;
    return 0;
}


HttpRequest *HttpRequest::parse(std::string &request) {
    std::cout << request << std::endl;
    std::istringstream stream(request, std::ios::binary);
    std::string line;

    std::getline(stream, line, '\n');
    std::istringstream requestLineStream(line);

    std::string temp;

    requestLineStream >> _method >> _path >> _version;

    parseParams(_path);

    bool headersComplete = false; // Flag to indicate if headers are complete
    bool isFile = false;

    while (std::getline(stream, line, '\n')) {
        if (!headersComplete) {
            // Find the position of the colon in the line
            std::size_t colonPos = line.find(':');

            if (colonPos != std::string::npos) {
                // Extract header and value
                std::string header = stringToLower(trim(line.substr(0, colonPos)));
                std::string value = trim(line.substr(colonPos + 1));

                this->setHeader(header, value);
            } else if (line == "\r") {
                // Empty line indicates end of headers
                headersComplete = true;
            }
        } else {
            // After headers, the remaining data is the body
            if (line == "\r" && !isFile) // Is its a multi-part, each part will have a \r\n line to determine the end of the part headers. This only works for one file upload :)Z
            {
                if (_body.find("Content-Disposition: form-data;") != std::string::npos &&
                    _body.find("filename=\"") != std::string::npos) {
                    // Extract the filename from the Content-Disposition header
                    size_t filenameStart = _body.find("filename=\"") + 10; // Length of "filename=\""
                    size_t filenameEnd = _body.find("\"", filenameStart);
                    std::string myValue = _body.substr(filenameStart, filenameEnd - filenameStart);
                    std::string myKey = "File-Name";
                    this->setHeader(myKey, myValue);
                    _body.clear();
                }
                continue;
            }
            _body.append(line);
            _body.append("\n"); // Append newline as it's removed by std::getline
        }
    }
    if (_method == "POST" && _headers.find("content-disposition") == _headers.end())
        parseParams(_body);
    printHttpRequest();
    return this;
}

int HttpRequest::getFd() const {
    return _serverFd;
}

void HttpRequest::setFd(int fd) {
    _serverFd = fd;
}
