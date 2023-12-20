//
// Created by Yash on 19/12/23.
//

#include "PollManager/PollManager.hpp"

PollManager::PollManager() {
}

PollManager::~PollManager() {}

PollManager::PollManager(const PollManager &src) {
	*this = src;
}

PollManager &PollManager::operator=(const PollManager &src) {
	if (this != &src) {
		this->_servers = src._servers;
	}
	return *this;
}

void PollManager::SocketConfig(const std::vector<ServerConfig> &Servers){
	for (unsigned long i = 0; i < Servers.size(); ++i) {
		int j = socket(AF_INET, SOCK_STREAM, 0);
		if(j == -1)
		{
			LOG_ERROR("Socket creation failed");
			for (unsigned long iter = 0; iter < sockets.size(); iter++) {
				close(sockets[iter]);
				LOG_INFO("Socket " << sockets[iter] << " closed");
			}
		}
		sockets.push_back(j);
	}
	for (unsigned long i = 0; i < Servers.size(); ++i) {
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = inet_addr(Servers[i].getListen()[0].first.c_str());
		server.sin_port = htons(Servers[i].getListen()[0].second);
		_servers.push_back(server);
	}
}

void PollManager::binder(const std::vector<ServerConfig> &Servers) {
	if(_servers.size() != sockets.size())
		LOG_ERROR("Servers and sockets size mismatch");
	for (unsigned long i = 0; i < _servers.size(); ++i) {
		if (bind(sockets[i], (struct sockaddr *)&_servers[i], sizeof(_servers[i])) < 0)
		{
			LOG_ERROR("Socket binding failed");
			for (unsigned long iter = 0; iter < sockets.size(); iter++) {
				close(sockets[iter]);
				LOG_INFO("Socket " << sockets[iter] << " closed");
			}
		}
	}
	for (unsigned long i = 0; i < sockets.size(); i++)
		LOG_INFO("Socket " << sockets[i] << " bound to " << Servers[i].getListen()[0].first << ":" << Servers[i].getListen()[0].second);
}
