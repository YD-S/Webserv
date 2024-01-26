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
	this->fd = fd;
	this->request = "";
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
	this->ip = ntohs(addr.sin_addr.s_addr);
	this->port = to_string(addr.sin_port);
	this->addr = addr;
}

Client &Client::operator=(const Client &src) {
	if (this != &src) {
		this->port = src.port;
		this->request = src.request;
		this->ip = src.ip;
		this->fd = src.fd;
		this->addr = src.addr;
	}
	return *this;
}

void Client::setPort(std::string port) {
	this->port = port;
}

void Client::setRequest(std::string request) {
	this->request = request;
}

void Client::setIp(int ip) {
	this->ip = ip;
}

void Client::setFd(int fd) {
	this->fd = fd;
}

void Client::setAddr(struct sockaddr_in addr) {
	this->addr = addr;
}

std::string Client::getPort() {
	return this->port;
}

std::string Client::getRequest() {
	return this->request;
}

int Client::getIp() {
	return this->ip;
}

int Client::getFd() const {
	return this->fd;
}

struct sockaddr_in Client::getAddr() {
	return this->addr;
}
