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

void PollManager::socketConfig(const std::vector<ServerConfig> &serversConfig){
	for (unsigned long i = 0; i < serversConfig.size(); ++i) {
		int j = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(j, SOL_SOCKET, SO_REUSEADDR, &opt,sizeof(int));
        setsockopt(j, SOL_SOCKET, SO_REUSEPORT, &opt,sizeof(int));
        setsockopt(j, SOL_SOCKET, SO_KEEPALIVE, &opt,sizeof(int));
		if(j < 0)
		{
			LOG_ERROR("Socket creation failed");
			kill(getpid(), SIGINT);
		}
		sockets.push_back(j);
	}
	for (unsigned long i = 0; i < serversConfig.size(); ++i) {
		struct sockaddr_in server;
		server.sin_family = AF_INET;
		for(unsigned long j = 0; j < serversConfig[i].getListen().size(); j++) {
			if (serversConfig[i].getListen()[j].first == "localhost")
				server.sin_addr.s_addr = inet_addr("127.0.0.1");
			else if (serversConfig[i].getListen()[j].first == "0.0.0.0")
				server.sin_addr.s_addr = INADDR_ANY;
			else
				server.sin_addr.s_addr = inet_addr(serversConfig[i].getListen()[j].first.c_str());
			server.sin_port = htons(serversConfig[i].getListen()[j].second);
		}
		_servers.push_back(server);
	}
}

void PollManager::binder(const std::vector<ServerConfig> &servers) {
	if(_servers.size() != sockets.size()) {
		LOG_ERROR("Servers and sockets size mismatch");
		kill(getpid(), SIGINT);
	}
	for (unsigned long i = 0; i < _servers.size(); i++) {
		if (bind(sockets[i], (struct sockaddr *)&_servers[i], sizeof(struct sockaddr_in)) == -1)
		{
			LOG_ERROR("Bind failed: " << errno);
			kill(getpid(), SIGINT);
		}
	}
	for (unsigned long i = 0; i < sockets.size(); i++)
		LOG_INFO("Socket " << sockets[i] << " is bound to " << servers[i].getListen()[0].first << ":" << servers[i].getListen()[0].second);
	for(unsigned long i = 0; i < sockets.size(); i++){
		if (listen(sockets[i], 25) < 0)
		{
			LOG_ERROR("Listen failed");
            kill(getpid(), SIGINT);
		}

	}
}

void PollManager::poller(unused std::vector<ServerConfig> &servers) {
    fd_set write_fd;
	fd_set read_fd;
	fd_set error_fd;
	FD_ZERO(&write_fd);
	FD_ZERO(&read_fd);
	FD_ZERO(&error_fd);
    int max_fd = 0;
    FILL_SET(sockets, sockets.at(i), read_fd, max_fd);
    FILL_SET(sockets, sockets.at(i), error_fd, max_fd);
    FILL_SET(clients, clients.at(i).getFd(), write_fd, max_fd);
    FILL_SET(clients, clients.at(i).getFd(), error_fd, max_fd);

//	for (unsigned long i = 0; i < sockets.size(); i++) {
//        if (sockets.at(i) > max_fd) {
//            max_fd = sockets.at(i);
//        }
//        FD_SET(sockets.at(i), &fds);
//    }
//	for (unsigned long i = 0; i < clients.size(); i++) {
//        if (clients.at(i).getFd() > max_fd) {
//            max_fd = clients.at(i).getFd();
//        }
//        FD_SET(clients.at(i).getFd(), &write_fd);
//    }
    timeval timeout = {0, 0};
	if (select(max_fd + 1, &read_fd, &write_fd, &error_fd, &timeout) < 0) {
        LOG_ERROR("Select failed: " << errno);
    }
	for(unsigned long i = 0; i < sockets.size(); i++){
		if(FD_ISSET(sockets[i], &read_fd)){
			socklen_t Size_Client = sizeof(clientData);
            clientSocket = accept(sockets[i], (struct sockaddr *)&clientData, &Size_Client);
			if (clientSocket < 0)
			{
                LOG_ERROR("Accept failed: " << errno);
                continue;
			}
			LOG_DEBUG("Socket " << clientSocket << " accepted");
			char buffer[1024] = {0};
			int readValue = read(clientSocket, buffer, 1024);
			if (readValue < 0)
			{
                LOG_ERROR("Read failed: " << errno);
                continue;
			}
			Client client = Client(clientSocket, clientData);
			clients.push_back(client);
			HttpRequest request = HttpRequest();
			request.parse(buffer);
			_requests.push_back(std::make_pair(&request, &client));
		}
	}

    std::vector<const HttpResponse *> responsesSent;
    for (unsigned long i = 0; i < _responses.size(); i++) {
        const HttpResponse *response = _responses[i].first;
        const Client *client = _responses[i].second;
        if (!FD_ISSET(client->getFd(), &write_fd))
            continue;
        responsesSent.push_back(response);
        std::string responseString = response->toRawString();
        send(client->getFd(), responseString.c_str(), responseString.length(), MSG_DONTWAIT);
        close(client->getFd());
        // Delete the connection from the clients vector
        for (unsigned long j = 0; j < clients.size(); j++) {
            if (clients[j].getFd() == client->getFd()) {
                clients.erase(clients.begin() + j);
                break;
            }
        }
    }
    // This can probably be optimized
    for (unsigned long i = 0; i < responsesSent.size(); i++) {
        for (unsigned long j = 0; j < _responses.size(); j++) {
            if (_responses[j].first == responsesSent[i]) {
                _responses.erase(_responses.begin() + j);
                break;
            }
        }
    }
    LOG_DEBUG("Remaining responses: " << _responses.size());
}

std::vector<std::pair<const HttpRequest *, const Client *> > PollManager::getRequests() {
	return _requests;
}

void PollManager::setResponses(std::vector<std::pair<const HttpResponse *, const Client *> > responses) {
	for (std::vector<std::pair<const HttpResponse *, const Client *> >::iterator it = responses.begin(); it != responses.end(); ++it) {
		_responses.push_back(*it);
	}
}

void PollManager::setRequestHandled(const HttpRequest *request) {
    for (std::vector<std::pair<const HttpRequest *, const Client *> >::iterator it = _requests.begin(); it != _requests.end(); ++it) {
        if (it->first == request) {
            _requests.erase(it);
            return;
        }
    }
}

std::vector<std::pair<const HttpResponse *, const Client *> > PollManager::getResponses() {
    return _responses;
}

