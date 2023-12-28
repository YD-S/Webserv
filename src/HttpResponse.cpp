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

static bool startsWith(const std::string& mainStr, const std::string& searchStr) {
    if (mainStr.length() < searchStr.length()) {
        // The main string is shorter than the search string, so it can't start with it.
        return false;
    }

    // Compare the substring of the main string with the search string
    return mainStr.substr(0, searchStr.length()) == searchStr;
}

static int fileExists(const std::string filename) {
    FILE* file = std::fopen(filename.c_str(), "r");
    if (file != NULL) {
        std::fclose(file);
        return 1;
    }

    // Check if the failure is due to file not existing or permission issues
    if (errno == ENOENT) {
        return 0;
    } else {
        return -1;
    }

    return false;
}

int HttpResponse::findStatus(HttpRequest &request, ServerConfig &config){
    int status;
    if (request.getMethod() == "GET"){
        for (std::vector<LocationConfig>::const_iterator it = config.getLocations().begin(); it != config.getLocations().end(); ++it){
            if (startsWith((*it).getRoot(), request.getPath())){
                if (!((*it).hasMethod("GET")))
                    return (401);
                status = fileExists((*it).getPath().substr(request.getPath().length()));
                if (status == 1) // Needs an additional check (400) requested variable doesnt exist, but the path does.
                    return 200;
                else if (!status)
                    return 404;
                else
                    return 403;
            }
        }
        return 404;
    }
    if (request.getMethod() == "POST"){
        for (std::vector<LocationConfig>::const_iterator it = config.getLocations().begin(); it != config.getLocations().end(); ++it){
            if (startsWith((*it).getRoot(), request.getPath())){
                if (!((*it).hasMethod("POST")))
                    return (401);
                status = fileExists((*it).getPath().substr(request.getPath().length()));
                if (status == 1) // Needs some additional checks, like the return of 200 (result of operator)
                    return 201; // 204 when variables change but no response
                else if (!status) // (400) requested variable doesnt exist, but the path does.
                    return 404;
                else
                    return 403;
            }
        }
        return 404;
    }
    if (request.getMethod() == "DELETE"){
        for (std::vector<LocationConfig>::const_iterator it = config.getLocations().begin(); it != config.getLocations().end(); ++it){
            if (startsWith((*it).getRoot(), request.getPath())){
                if (!((*it).hasMethod("DELETE")))
                    return (401);
                status = fileExists((*it).getPath().substr(request.getPath().length()));
                if (status == 1)
                    return 204;
                else if (!status)
                    return 404;
                else
                    return 403;
            }
        }
        return 404;
    }
    return 501;
}