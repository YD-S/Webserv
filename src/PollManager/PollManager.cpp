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

void PollManager::SocketConfig(const std::vector<ServerConfig> &Servers_Config){
	for (unsigned long i = 0; i < Servers_Config.size(); ++i) {
		int j = socket(AF_INET, SOCK_STREAM, 0);
		if(j < 0)
		{
			LOG_ERROR("Socket creation failed");
			kill(getpid(), SIGINT);
		}
		sockets.push_back(j);
	}
	for (unsigned long i = 0; i < Servers_Config.size(); ++i) {
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		for(unsigned long j = 0; j < Servers_Config[i].getListen().size(); j++) {
			if (Servers_Config[i].getListen()[j].first == "localhost")
				server.sin_addr.s_addr = inet_addr("127.0.0.1");
			else if (Servers_Config[i].getListen()[j].first == "0.0.0.0")
				server.sin_addr.s_addr = INADDR_ANY;
			else
				server.sin_addr.s_addr = inet_addr(Servers_Config[i].getListen()[j].first.c_str());
			server.sin_port = htons(Servers_Config[i].getListen()[j].second);
		}
		_servers.push_back(server);
	}
}

void PollManager::Binder(const std::vector<ServerConfig> &Servers) {
	if(_servers.size() != sockets.size()) {
		LOG_ERROR("Servers and sockets size mismatch");
		kill(getpid(), SIGINT);
	}
	for (unsigned long i = 0; i < _servers.size(); i++) {
		if (bind(sockets[i], (struct sockaddr *)&_servers[i], sizeof(struct sockaddr_in)) == -1)
		{
			LOG_ERROR("Socket binding failed");
			kill(getpid(), SIGINT);
		}
	}
	for (unsigned long i = 0; i < sockets.size(); i++)
		LOG_INFO("Socket " << sockets[i] << " is bound to " << Servers[i].getListen()[0].first << ":" << Servers[i].getListen()[0].second);
	for(unsigned long i = 0; i < sockets.size(); i++){
		if (listen(sockets[i], 25) < 0)
		{
			LOG_ERROR("Socket listening failed");
			kill(getpid(), SIGINT);
		}

	}
}

void PollManager::Poller(std::vector<ServerConfig> &servers) {
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
			kill(getpid(), SIGINT);
		}
		for(unsigned long i = 0; i < sockets.size(); i++){
			if(FD_ISSET(sockets[i], &read_fd)){
				struct sockaddr_in Client_Data;
				socklen_t Size_Client = sizeof(Client_Data);
				int client_socket = accept(sockets[i], (struct sockaddr *)&Client_Data, &Size_Client);
				if (client_socket < 0)
				{
					LOG_ERROR("Socket accept failed");
					kill(getpid(), SIGINT);
				}
				LOG_INFO("Socket " << client_socket << " accepted");
				char buffer[1024] = {0};
				int Read_value = read(client_socket, buffer, 1024);
				if (Read_value < 0)
				{
					LOG_ERROR("Socket read failed");
					kill(getpid(), SIGINT);
				}
				Clients client = Clients(client_socket, Client_Data);
				clients.push_back(client);
				std::cout << "Server: " << ft_socket_to_string(_servers[i]) << " Has accepeted a client "  << ft_socket_to_string(client.getAddr()) << std::endl;

				HttpRequest request = HttpRequest();
				request.parse(buffer);
				HttpResponse response = HttpResponse();
				response.build(request, servers[i]);
				std::string responseString = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";
				LOG_INFO("Socket " << client_socket << " read");
				LOG_INFO("Socket " << client_socket << " read: " << buffer);
				send(client_socket, responseString.c_str(), responseString.length(), 0);
				LOG_INFO("Socket " << client_socket << " sent");
				close(client_socket);
				LOG_INFO("Socket " << client_socket << " closed");
			}
		}
	}
}

HttpResponse PollManager::Response_Builder(HttpResponse response) {
	response.setVersion("HTTP/1.1");
	switch (response.getStatus()){
		case 200:
			response.addHeader("Status", "200 OK");
			break;
		case 201:
			response.addHeader("Status", "201 Created");
			break;
		case 202:
			response.addHeader("Status", "202 Accepted");
			break;
		case 203:
			response.addHeader("Status", "203 Non-Authoritative Information");
			break;
		case 204:
			response.addHeader("Status", "204 No Content");
			break;
		case 205:
			response.addHeader("Status", "205 Reset Content");
			break;
		case 206:
			response.addHeader("Status", "206 Partial Content");
			break;
		case 300:
			response.addHeader("Status", "300 Multiple Choices");
			break;
		case 301:
			response.addHeader("Status", "301 Moved Permanently");
			break;
		case 302:
			response.addHeader("Status", "302 Found");
			break;
		case 308:
			response.addHeader("Status", "308 Permanent Redirect");
			break;
		case 400:
			response.addHeader("Status", "400 Bad Request");
			break;
		case 401:
			response.addHeader("Status", "401 Unauthorized");
			break;
		case 403:
			response.addHeader("Status", "403 Forbidden");
			break;
		case 404:
			response.addHeader("Status", "404 Not Found");
			break;
		case 408:
			response.addHeader("Status", "408 Request Timeout");
			break;
		case 501:
			response.addHeader("Status", "501 Not Implemented");
			break;
		case 502:
			response.addHeader("Status", "502 Bad Gateway");
			break;
		case 504:
			response.addHeader("Status", "504 Gateway Timeout");
			break;
		default:
			response.addHeader("Status", "500 Internal Server Error");
			break;
	}
	return response;
}

