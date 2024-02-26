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

HttpResponse *HttpResponse::setVersion(const std::string &version) {
	this->_version = version;
	return this;
}

HttpResponse *HttpResponse::setStatus(int status) {
	this->_status = status;
	return this;
}

HttpResponse *HttpResponse::setHeader(const std::string &key, const std::string &value) {
	this->_headers.insert(std::make_pair(stringToLower(key), value));
	return this;
}

HttpResponse *HttpResponse::setBody(const std::string &body) {
	this->_body = body;
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

const std::string &HttpResponse::getBody() const {
	return _body;
}

std::string HttpResponse::toRawString() const {
	std::string response =
			this->_version + " " + to_string(this->_status) + " " + HttpStatus::getReasonString(this->_status) + "\r\n";
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin();
		 it != this->_headers.end(); ++it) {
		response += it->first + ": " + it->second + "\r\n";
	}
	response += "\r\n";
	response += this->_body;
	response += "\r\n";
	return response;
}

std::string HttpResponse::toPrintableString() const {
	std::string response = this->toRawString();
	std::string printableResponse = "";
	bool reachedBody = false;
	// replace \r\n with \n
	for (std::string::iterator it = response.begin(); it != response.end(); ++it) {
		// if we reach the CRLFCRLF, we print the rest of the response as is
		if (*it == '\r' && *(it + 1) == '\n' && *(it + 2) == '\r' && *(it + 3) == '\n') {
			reachedBody = true;
			printableResponse += "\n\n";
			it += 4;
		}
		if (*it != '\r' && !reachedBody)
			printableResponse += *it;
		else if (reachedBody) {
			// replace unprintable characters with their hex value
			if (*it < 32 || *it > 126)
				printableResponse += "\\x" + to_string((int) *it);
			else
				printableResponse += *it;
		}
	}
	return printableResponse;
}

void HttpResponse::parseStatusLine(std::string statusLine) {
	// Assuming the status line format is "HTTP/1.1 200 OK\r"
	std::size_t firstSpacePos = statusLine.find(' ');
	std::size_t secondSpacePos = statusLine.find(' ', firstSpacePos + 1);
	if (statusLine[statusLine.length() - 1] != '\r')
		LOG_WARNING("Status line does not end with \\r");
	else
		statusLine = statusLine.substr(0, statusLine.length() - 1);
	if (firstSpacePos != std::string::npos && secondSpacePos != std::string::npos) {
		std::string version = statusLine.substr(0, firstSpacePos);
		std::string status = statusLine.substr(firstSpacePos + 1, secondSpacePos - firstSpacePos - 1);
		this->setVersion(version);
		this->setStatus(ft_stoi(status));
	}
}

void HttpResponse::parseHeader(std::string headerLine) {
	std::size_t colonPos = headerLine.find(':');
	if (headerLine[headerLine.length() - 1] != '\r')
		LOG_WARNING("Header line does not end with \\r");
	else
		headerLine = headerLine.substr(0, headerLine.length() - 1);
	if (colonPos != std::string::npos) {
		std::string key = headerLine.substr(0, colonPos);
		std::string value = trim(headerLine.substr(colonPos + 1));
		setHeader(key, value);
	}
}

void HttpResponse::fromString(const std::string &str) {
	std::istringstream responseStream(str);
	std::string line;
	size_t pos = 0;

	// Parse the first line to get version and status
	if (std::getline(responseStream, line)) {
		pos += line.length() + 1;
		parseStatusLine(line);
	}

	// Parse headers
	while (std::getline(responseStream, line) && !line.empty() && line != "\r") {
		pos += line.length() + 1;
		parseHeader(line);
	}
	pos += 2; // Skip \r\n

	// Parse body
	std::string body = str.substr(pos);
	setBody(body);
}