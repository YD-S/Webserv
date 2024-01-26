//
// Created by Yash on 29/12/23.
//

#ifndef WEBSERV_CLIENT_HPP
#define WEBSERV_CLIENT_HPP
#include "utils.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>


class Client {
private:
	std::string port;
	std::string request;
	int ip;
	int fd;
	struct sockaddr_in addr;
	Client();

public:
	~Client();
	Client(const Client &src);
	Client(int fd, struct sockaddr_in addr);
	Client &operator=(const Client &src);
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


#endif //WEBSERV_CLIENT_HPP
