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

HttpResponse *HttpResponse::setVersion(const std::string &_version) {
	this->_version = _version;
	return this;
}

HttpResponse *HttpResponse::setStatus(int _status) {
	this->_status = _status;
	return this;
}

HttpResponse *HttpResponse::setHeader(const std::string &key, const std::string &value) {
	this->_headers.insert(std::make_pair(key, value));
	return this;
}

HttpResponse *HttpResponse::setBody(const std::string &_body) {
	this->setHeader("Content-length", to_string(_body.size()));
	for (std::string::const_iterator it = _body.begin(); it != _body.end(); ++it) {
		this->_body.push_back(*it);
	}
	return this;
}

HttpResponse *HttpResponse::setBody(const std::vector<char> &_body) {
	this->setHeader("Content-length", to_string(_body.size()));
	for (std::vector<char>::const_iterator it = _body.begin(); it != _body.end(); ++it) {
		this->_body.push_back(*it);
	}
	return this;
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

const std::vector<char> & HttpResponse::getBody() const {
	return _body;
}

std::string HttpResponse::toRawString() const {
	std::string resp = this->_version + " " + to_string(this->_status) + " " + HttpStatus::getReasonString(this->_status) + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
		 it != this->_headers.end(); ++it) {
		resp += it->first + ": " + it->second + "\r\n";
	}
	resp += "\r\n";
	for (std::vector<char>::const_iterator it = this->_body.begin(); it != this->_body.end(); ++it) {
		resp.push_back(*it);
	}
	resp += "\r\n";
	return resp;
}

void HttpResponse::getContentType() {

}

bool HttpResponse::isCGI(std::string &path) {
	int extPoint = path.find_last_of(".");
	if (path.substr(extPoint) == ".py" || path.substr(extPoint) == ".sh")
		return true;
	return false;
}

void HttpResponse::printAll() const {
	std::cout << "Version: " << _version << std::endl;
	std::cout << "Status: " << _status << std::endl;

	// Print headers
	std::cout << "Headers:" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); ++it) {
		std::cout << "  " << it->first << ": " << it->second << std::endl;
	}

	std::cout << "Body: " << &_body[0] << std::endl;
}