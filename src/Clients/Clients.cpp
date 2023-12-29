//
// Created by Yash on 29/12/23.
//

#include "Clients/Clients.hpp"

Clients::Clients() {}

Clients::~Clients() {}

Clients::Clients(const Clients &src) {
	*this = src;
}

Clients::Clients(int fd, struct sockaddr_in addr) {
	this->fd = fd;
	this->request = "";
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr.sin_addr), str, INET_ADDRSTRLEN);
	this->ip = ntohs(addr.sin_addr.s_addr);
	this->port = to_string(addr.sin_port);
	this->addr = addr;
}

Clients &Clients::operator=(const Clients &src) {
	if (this != &src) {
		this->port = src.port;
		this->request = src.request;
		this->ip = src.ip;
		this->fd = src.fd;
		this->addr = src.addr;
	}
	return *this;
}

void Clients::setPort(std::string port) {
	this->port = port;
}

void Clients::setRequest(std::string request) {
	this->request = request;
}

void Clients::setIp(int ip) {
	this->ip = ip;
}

void Clients::setFd(int fd) {
	this->fd = fd;
}

void Clients::setAddr(struct sockaddr_in addr) {
	this->addr = addr;
}

std::string Clients::getPort() {
	return this->port;
}

std::string Clients::getRequest() {
	return this->request;
}

int Clients::getIp() {
	return this->ip;
}

int Clients::getFd() {
	return this->fd;
}

struct sockaddr_in Clients::getAddr() {
	return this->addr;
}
