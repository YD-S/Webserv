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

HttpRequest HttpRequest::setMethod(const std::string &method) {
    this->_method = method;
    return *this;
}

HttpRequest HttpRequest::setPath(const std::string &path) {
    this->_path = path;
    return *this;
}

HttpRequest HttpRequest::setVersion(const std::string &version) {
    this->_version = version;
    return *this;
}

HttpRequest HttpRequest::addHeader(const std::string &key, const std::string &value) {
    _headers.insert(std::make_pair(key, value));
    return *this;
}

const HttpRequest &HttpRequest::setBody(const std::string &_body) {
    this->_body = _body;
    return *this;
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

const std::string &HttpRequest::getBody() const {
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

HttpRequest HttpRequest::addParam(const std::string &key, const std::string &value) {
    _params.insert(std::make_pair(key, value));
    return *this;
}

const std::string &HttpRequest::getHeader(const std::string &key) const {
    return _headers.find(key)->second;
}

const std::string &HttpRequest::getParam(const std::string &key) const {
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

static std::string trim(const std::string& str) {
    std::size_t first = str.find_first_not_of(" \t\r");
    if (first == std::string::npos)
        return "";

    std::size_t last = str.find_last_not_of(" \t\r");
    return str.substr(first, last - first + 1);
}

void HttpRequest::parsePostParams(std::istringstream &stream) {
    std::string param;

    while (std::getline(stream, param, '&')) {
        // Split each parameter into key and value
        size_t equal_pos = param.find('=');
        if (equal_pos != std::string::npos) {
            std::string key = param.substr(0, equal_pos);
            std::string value = param.substr(equal_pos + 1);
            _params[key] = value;
        }
        else
            _body.append(param);
    }
}

void HttpRequest::parseGetParams(const std::string& path_string) {

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

        std::cout << "Body: " << _body << std::endl;
    }

HttpRequest HttpRequest::parse(std::string request){
    std::istringstream stream(request);
    std::string line;

    std::getline(stream, line, '\n');
    std::istringstream requestLineStream(line);

    std::string temp;

    requestLineStream >> _method >> _path >> _version;

    if (_method == "GET")
        parseGetParams(_path);

    while (std::getline(stream, line, '\n')) {
        // Find the position of the colon in the line
        std::size_t colonPos = line.find(':');

        if (colonPos != std::string::npos) {
            // Extract header and value
            std::string header = trim(line.substr(0, colonPos));
            std::string value = trim(line.substr(colonPos + 1));

            this->addHeader(header, value);
            // Print the parsed header and value
//            std::cout << "Header: '" << header << "', Value: '" << value << "'" << std::endl;
        } else {
            if (_method == "POST")
                parsePostParams(stream);
            else if (_method == "GET" || _method == "DELETE")
                std::getline(stream, _body, '\0');
            else
                ft_error("Unkown method!", 1);
        }
    }
//    printHttpRequest();
    return *this;
}

int HttpRequest::getFd() const {
    return _serverFd;
}

void HttpRequest::setFd(int fd) {
    _serverFd = fd;
}
