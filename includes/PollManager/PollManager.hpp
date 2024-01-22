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
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "HttpStatus.hpp"
#include "Clients/Clients.hpp"

extern std::vector<int> sockets;
extern std::vector<Clients> clients;

class PollManager {
private:
	std::vector<struct sockaddr_in> _servers;
	int client_socket;
	struct sockaddr_in Client_Data;
	std::vector<std::pair<const HttpRequest *, const Clients *> > _requests;
	std::vector<std::pair<const HttpResponse *, const Clients *> > _responses;

public:
	PollManager();
	~PollManager();
	PollManager(const PollManager &src);
	PollManager &operator=(const PollManager &src);
	void SocketConfig(const std::vector<ServerConfig> &Servers_Config);
	void Binder(const std::vector<ServerConfig> &Servers);
	void Poller(std::vector<ServerConfig> &Servers);
	HttpResponse Response_Builder(HttpResponse response);

	std::vector<std::pair<const HttpRequest *, const Clients *> > getRequests();
    void setResponses(std::vector<std::pair<const HttpResponse *, const Clients *> > responses);
};

#endif //WEBSERV_POLLMANAGER_HPP
