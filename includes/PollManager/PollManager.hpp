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
#include "Client/Client.hpp"

extern std::vector<int> sockets;
extern std::vector<Client> clients;

class PollManager {
private:
	std::vector<struct sockaddr_in> _servers;
	int clientSocket;
	struct sockaddr_in clientData;
	std::vector<std::pair<const HttpRequest *, const Client *> > _requests;
	std::vector<std::pair<const HttpResponse *, const Client *> > _responses;

public:
	PollManager();
	~PollManager();
	PollManager(const PollManager &src);
	PollManager &operator=(const PollManager &src);
	void socketConfig(const std::vector<ServerConfig> &Servers_Config);
	void binder(const std::vector<ServerConfig> &Servers);
	void Poller(std::vector<ServerConfig> &Servers);

	std::vector<std::pair<const HttpRequest *, const Client *> > getRequests();
    void setResponses(std::vector<std::pair<const HttpResponse *, const Client *> > responses);
    void setRequestHandled(const HttpRequest *request);
    std::vector<std::pair<const HttpResponse *, const Client *> > getResponses();
};

#endif //WEBSERV_POLLMANAGER_HPP
