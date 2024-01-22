//
// Created by kolterdyx on 14/09/23.
//

#include "HttpResponse.hpp"
#include "HttpStatus.hpp"
#include "utils.hpp"

HttpResponse::HttpResponse() : _status(HttpStatus::IM_A_TEAPOT) {
    this->_version = "HTTP/1.1";
}

HttpResponse::HttpResponse(const HttpResponse &other) : _status() {
    *this = other;
}

HttpResponse &HttpResponse::operator=(const HttpResponse &other) {
    if (this != &other) {
        this->_version = other._version;
        this->_status = other._status;
        this->_headers = other._headers;
        this->_body = other._body;
    }
    return *this;
}

HttpResponse::~HttpResponse() {

}

HttpResponse HttpResponse::setVersion(const std::string &_version) {
    this->_version = _version;
    return *this;
}

HttpResponse HttpResponse::setStatus(int _status) {
    this->_status = _status;
    return *this;
}

HttpResponse HttpResponse::addHeader(const std::string &key, const std::string &value) {
    this->_headers.insert(std::make_pair(key, value));
    return *this;
}

HttpResponse HttpResponse::setBody(const std::string &_body) {
    this->_body = _body;
    return *this;
}

const std::string &HttpResponse::getVersion() const {
    return _version;
}

int HttpResponse::getStatus() const {
    return _status;
}

const std::map<std::string, std::string> &HttpResponse::getHeaders() const {
    return _headers;
}

const std::string &HttpResponse::getBody() const {
    return _body;
}

std::string HttpResponse::toRawString() {
    std::string response = this->_version + " " + to_string(this->_status) + " " + HttpStatus::getReasonString(this->_status) + "\r\n";
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it) {
        response += it->first + ": " + it->second + "\r\n";
    }
    response += "\r\n";
    response += this->_body;
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

int HttpResponse::fileExists(std::string filename) {
    std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary);
    if (file != NULL) {
        if (!isCGI(filename)){
            if (!std::getline(file, _body, '\0'))
                return 500;
            addHeader("Content-length", to_string(_body.length()));
        }
        file.close();
        return 1;
    }

    // Check if the failure is due to file not existing or permission issues
    if (errno == ENOENT) {
        return 0;
    } else {
        return -1;
    }
}

void HttpResponse::getContentType(){
    
}

int HttpResponse::findStatus(HttpRequest &request, ServerConfig &config){
    if (request.getMethod() == "GET"){
        for (std::vector<LocationConfig>::const_iterator it = config.getLocations().begin(); it != config.getLocations().end(); ++it){
			std::string combinedPath = (it)->getRoot() + request.getPath();
			LOG_ERROR(combinedPath);
				if (combinedPath.at(combinedPath.length() - 1) == '/')
					combinedPath = combinedPath.substr(1);
			// LOG_ERROR(combinedPath);
				// if (startsWith((it).getRoot(), request.getPath())){
				if (!(*it).hasMethod("GET"))
					return 401;
				if (request.getPath().empty() || request.getPath().at(0) != '/')
					return 402;

				if (combinedPath.size() == 1)
					return 200; // Assuming this is the intended behavior for path size 1

				int _status = fileExists(combinedPath);
				if (_status == 1)
					return 200;
				else if (!_status)
					return 404;
				else
					return 403;
				// }
			}
			return 500;
    }
    if (request.getMethod() == "POST"){
        for (std::vector<LocationConfig>::const_iterator it = config.getLocations().begin(); it != config.getLocations().end(); ++it){
            if (startsWith((*it).getRoot(), request.getPath())){
                if (!((*it).hasMethod("POST")))
                    return (401);
                _status = fileExists((*it).getPath().substr(request.getPath().length()));
                if (_status == 1) // Needs some additional checks, like the return of 200 (result of operator)
                    return 201; // 204 when variables change but no response
                else if (!_status) // (400) requested variable doesnt exist, but the path does.
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
                _status = fileExists((*it).getPath().substr(request.getPath().length()));
                if (_status == 1)
                    return 204;
                else if (!_status)
                    return 404;
                else
                    return 403;
            }
        }
        return 404;
    }
    return 501;
}

bool    HttpResponse::isCGI(std::string &path){
    int extPoint = path.find_last_of(".");
    if (path.substr(extPoint) == ".py" || path.substr(extPoint) == ".sh")
        return true;
    return false;
}

void    HttpResponse::build(HttpRequest &request, ServerConfig &server){ // Need CGI, ContentType header.
    LOG_ERROR("STARTS HERE");
    request.printHttpRequest();
    LOG_ERROR("ENDS HERE");
    setVersion("HTTP/1.1");
    setStatus(findStatus(request, server));
    //if (isCGI(request.getPath()))
    //    _body = CGI();
    LOG_ERROR("STARTS HERE");
    printAll();
    LOG_ERROR("ENDS HERE");
}

void HttpResponse::printAll() const {
        std::cout << "Version: " << _version << std::endl;
        std::cout << "Status: " << _status << std::endl;

        // Print headers
        std::cout << "Headers:" << std::endl;
        for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
            std::cout << "  " << it->first << ": " << it->second << std::endl;
        }

        std::cout << "Body: " << _body << std::endl;
}