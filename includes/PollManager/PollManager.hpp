//
// Created by Yash on 19/12/23.
//

#ifndef WEBSERV_POLLMANAGER_HPP
#define WEBSERV_POLLMANAGER_HPP

#include "config/ServerConfig.hpp"
#include "utils.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <algorithm>
#include <csignal>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
#include <utility>
#include <string>
#include <stack>

extern std::vector<int> sockets;

class PollManager {
private:
	std::vector<struct sockaddr_in> _servers;

public:
	PollManager();
	~PollManager();
	PollManager(const PollManager &src);
	PollManager &operator=(const PollManager &src);
	void SocketConfig(const std::vector<ServerConfig> &Servers_Config);
	void Binder(const std::vector<ServerConfig> &Servers);
	void Poller();
};

#endif //WEBSERV_POLLMANAGER_HPP
