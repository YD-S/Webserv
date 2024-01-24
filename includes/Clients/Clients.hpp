//
// Created by Yash on 29/12/23.
//

#ifndef WEBSERV_CLIENTS_HPP
#define WEBSERV_CLIENTS_HPP
#include "utils.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>


class Clients {
private:
	std::string port;
	std::string request;
	int ip;
	int fd;
	struct sockaddr_in addr;
	Clients();

public:
	~Clients();
	Clients(const Clients &src);
	Clients(int fd, struct sockaddr_in addr);
	Clients &operator=(const Clients &src);
	void setPort(std::string port);
	void setRequest(std::string request);
	void setIp(int ip);
	void setFd(int fd);
	void setAddr(struct sockaddr_in addr);
	std::string getPort();
	std::string getRequest();
	int getIp();
	int getFd() const;
	struct sockaddr_in getAddr();
};


#endif //WEBSERV_CLIENTS_HPP
