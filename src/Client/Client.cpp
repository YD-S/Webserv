//
// Created by Yash on 29/12/23.
//

#include "Client/Client.hpp"

Client::Client() {}

Client::~Client() {}

Client::Client(const Client &src) {
	*this = src;
}

Client::Client(int fd, struct sockaddr_in addr) {
	this->_fd = fd;
	this->_request = "";
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
	this->_ip = ntohs(addr.sin_addr.s_addr);
	this->_port = to_string(addr.sin_port);
	this->_addr = addr;
}

Client &Client::operator=(const Client &src) {
	if (this != &src) {
		this->_port = src._port;
		this->_request = src._request;
		this->_ip = src._ip;
		this->_fd = src._fd;
		this->_addr = src._addr;
	}
	return *this;
}

void Client::setPort(std::string port) {
	this->_port = port;
}

void Client::setRequest(std::string request) {
	this->_request = request;
}

void Client::setIp(int ip) {
	this->_ip = ip;
}

void Client::setFd(int fd) {
	this->_fd = fd;
}

void Client::setAddr(struct sockaddr_in addr) {
	this->_addr = addr;
}

std::string Client::getPort() {
	return this->_port;
}

std::string Client::getRequest() {
	return this->_request;
}

int Client::getIp() {
	return this->_ip;
}

int Client::getFd() const {
	return this->_fd;
}

struct sockaddr_in Client::getAddr() {
	return this->_addr;
}
