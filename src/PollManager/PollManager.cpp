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
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(Servers[i].getListen()[0].second);
		_servers.push_back(server);
	}
}

void PollManager::Binder(const std::vector<ServerConfig> &Servers) {
	if(_servers.size() != sockets.size())
		LOG_ERROR("Servers and sockets size mismatch");
	for (unsigned long i = 0; i < _servers.size(); i++) {
		if (bind(sockets[i], (struct sockaddr *)&_servers[i], sizeof(_servers[i])) == -1)
		{
			LOG_ERROR("Socket binding failed");
			for (unsigned long iter = 0; iter < sockets.size(); iter++) {
				close(sockets[iter]);
				LOG_INFO("Socket " << sockets[iter] << " closed");
			}
		}
	}
	for (unsigned long i = 0; i < sockets.size(); i++)
		LOG_INFO("Socket " << sockets[i] << " is bound to " << Servers[i].getListen()[0].first << ":" << Servers[i].getListen()[0].second);
	for(unsigned long i = 0; i < sockets.size(); i++){
		if (listen(sockets[i], 25) < 0)
		{
			LOG_ERROR("Socket listening failed");
			for (unsigned long iter = 0; iter < sockets.size(); iter++) {
				close(sockets[iter]);
				LOG_INFO("Socket " << sockets[iter] << " closed");
			}
		}

	}
}

void PollManager::Poller() {
	fd_set fds;
	FD_ZERO(&fds);
	for (unsigned long i = 0; i < sockets.size(); i++)
		FD_SET(sockets.at(i), &fds);
	int max_fd = *std::max_element(sockets.begin(), sockets.end());
	while(1){
		fd_set read_fd = fds;
		if (select(max_fd + 1, &read_fd, NULL, NULL, NULL) < 0)
		{
			LOG_ERROR("Socket select failed");
			for (unsigned long iter = 0; iter < sockets.size(); iter++) {
				close(sockets[iter]);
				LOG_INFO("Socket " << sockets[iter] << " closed");
			}
		}
		for(unsigned long i = 0; i < sockets.size(); i++){
			if(FD_ISSET(sockets[i], &read_fd)){
				struct sockaddr_in Client_Data;
				socklen_t Size_Client = sizeof(Client_Data);
				int client_socket = accept(sockets[i], (struct sockaddr *)&Client_Data, &Size_Client);
				if (client_socket < 0)
				{
					LOG_ERROR("Socket accept failed");
					for (unsigned long iter = 0; iter < sockets.size(); iter++) {
						close(sockets[iter]);
						LOG_INFO("Socket " << sockets[iter] << " closed");
					}
				}
				LOG_INFO("Socket " << client_socket << " accepted");
				char buffer[1024] = {0};
				int Read_value = read(client_socket, buffer, 1024);
				if (Read_value < 0)
				{
					LOG_ERROR("Socket read failed");
					for (unsigned long iter = 0; iter < sockets.size(); iter++) {
						close(sockets[iter]);
						LOG_INFO("Socket " << sockets[iter] << " closed");
					}
				}
				std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!$";
				LOG_INFO("Socket " << client_socket << " read");
				LOG_INFO("Socket " << client_socket << " read: " << buffer);
				send(client_socket, response.c_str(), response.length(), 0);
				LOG_INFO("Socket " << client_socket << " sent");
				close(client_socket);
				LOG_INFO("Socket " << client_socket << " closed");
			}
		}
	}
}