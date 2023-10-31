//
// Created by kolterdyx on 14/09/23.
//

#include "HttpResponse.hpp"
#include "HttpStatus.hpp"
#include "utils.hpp"

HttpResponse::HttpResponse() : status(HttpStatus::IM_A_TEAPOT) {
    this->version = "HTTP/1.1";
}

HttpResponse::HttpResponse(const HttpResponse &other) : status() {
    *this = other;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other) {
    if (this != &other) {
        this->version = other.version;
        this->status = other.status;
        this->headers = other.headers;
        this->body = other.body;
    }
    return *this;
}

HttpResponse::~HttpResponse() {

}

HttpResponse HttpResponse::setVersion(const std::string &version) {
    this->version = version;
    return *this;
}

HttpResponse HttpResponse::setStatus(int status) {
    this->status = status;
    return *this;
}

HttpResponse HttpResponse::addHeader(const std::string &key, const std::string &value) {
    this->headers.insert(std::make_pair(key, value));
    return *this;
}

HttpResponse HttpResponse::setBody(const std::string &body) {
    this->body = body;
    return *this;
}

const std::string &HttpResponse::getVersion() const {
    return version;
}

int HttpResponse::getStatus() const {
    return status;
}

const std::map<std::string, std::string> &HttpResponse::getHeaders() const {
    return headers;
}

const std::string &HttpResponse::getBody() const {
    return body;
}

std::string HttpResponse::toRawString() {
    std::string response = this->version + " " + to_string(this->status) + " " + HttpStatus::getReasonString(this->status) + "\r\n";
    for (std::map<std::string, std::string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n";
    response += this->body;
    return response;
}

std::string HttpResponse::toPrintableString() {
    std::string response = this->toRawString();
    // replace \r\n with \n
    for (std::string::iterator it = response.begin(); it != response.end(); ++it) {
        if (*it == '\r' && *(it + 1) == '\n') {
            *it = '\n';
            response.erase(it + 1);
        }
    }
    return response;
}
