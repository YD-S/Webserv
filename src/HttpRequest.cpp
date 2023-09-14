//
// Created by kolterdyx on 14/09/23.
//

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {
    this->version = "HTTP/1.1";
}

HttpRequest::~HttpRequest() {

}

HttpRequest HttpRequest::setMethod(const std::string &method) {
    this->method = method;
    return *this;
}

HttpRequest HttpRequest::setPath(const std::string &path) {
    this->path = path;
    return *this;
}

HttpRequest HttpRequest::setVersion(const std::string &version) {
    this->version = version;
    return *this;
}

HttpRequest HttpRequest::addHeader(const std::string &key, const std::string &value) {
    headers.push_back(std::make_pair(key, value));
    return *this;
}

const HttpRequest &HttpRequest::setBody(const std::string &body) {
    this->body = body;
    return *this;
}

const std::string &HttpRequest::getMethod() const {
    return method;
}

const std::string &HttpRequest::getPath() const {
    return path;
}

const std::string &HttpRequest::getVersion() const {
    return version;
}


const std::vector<std::pair<std::string, std::string> > &HttpRequest::getHeaders() const {
    return headers;
}

const std::string &HttpRequest::getBody() const {
    return body;
}

std::string HttpRequest::toRawString() {
    std::string rawString = method + " " + path + " " + version + "\r\n";
    for (std::vector<std::pair<std::string, std::string> >::iterator it = headers.begin(); it != headers.end(); ++it) {
        rawString += it->first + ": " + it->second + "\r\n";
    }
    rawString += "\r\n";
    rawString += body;
    return rawString;
}

std::string HttpRequest::toPrintableString() {
    std::string request = this->toRawString();
    // replace \r\n with \n
    std::string::size_type pos = 0;
    while ((pos = request.find("\r\n", pos)) != std::string::npos) {
        request.replace(pos, 2, "\n");
        pos += 1;
    }
    return request;
}
