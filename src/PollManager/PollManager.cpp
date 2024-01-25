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

void PollManager::socketConfig(const std::vector<ServerConfig> &Servers_Config){
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

void PollManager::binder(const std::vector<ServerConfig> &Servers) {
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
    fd_set write_fd;
    FD_ZERO(&fds);
	FD_ZERO(&fds);
    int max_fd = 0;
	for (unsigned long i = 0; i < sockets.size(); i++) {
        if (sockets.at(i) > max_fd) {
            max_fd = sockets.at(i);
        }
        FD_SET(sockets.at(i), &fds);
    }
	for (unsigned long i = 0; i < clients.size(); i++) {
        if (clients.at(i).getFd() > max_fd) {
            max_fd = clients.at(i).getFd();
        }
        FD_SET(clients.at(i).getFd(), &write_fd);
    }
	fd_set read_fd = fds;
	fd_set error_fd = fds;
    timeval timeout = {0, 0};
	if (select(max_fd + 1, &read_fd, &write_fd, &error_fd, &timeout) < 0) {
        LOG_DEBUG("Socket select failed");
    }
	for(unsigned long i = 0; i < sockets.size(); i++){
		if(FD_ISSET(sockets[i], &read_fd)){
			socklen_t Size_Client = sizeof(clientData);
            clientSocket = accept(sockets[i], (struct sockaddr *)&clientData, &Size_Client);
			if (clientSocket < 0)
			{
				LOG_ERROR("Socket accept failed");
                continue;
			}
			LOG_DEBUG("Socket " << clientSocket << " accepted");
			char buffer[1024] = {0};
			int readValue = read(clientSocket, buffer, 1024);
			if (readValue < 0)
			{
				LOG_DEBUG("Socket read failed");
                continue;
			}
			Clients client = Clients(clientSocket, clientData);
			clients.push_back(client);
			FD_SET(clientSocket, &fds);
			std::cout << "Server: " <<  servers[i].getServerName() << " Ip and Port " << ft_socket_to_string(_servers[i]) << " Has accepeted a client "  << ft_socket_to_string(client.getAddr()) << std::endl;
			HttpRequest request = HttpRequest();
			request.parse(buffer);
			_requests.push_back(std::make_pair(&request, &client));
		}
	}

    std::vector<const HttpResponse *> responsesSent;
    for (unsigned long i = 0; i < _responses.size(); i++) {
        const HttpResponse *response = _responses[i].first;
        const Clients *client = _responses[i].second;
        if (!FD_ISSET(client->getFd(), &write_fd)) {
            LOG_DEBUG("Socket " << client->getFd() << " is not ready to write");
            continue;
        }
        responsesSent.push_back(response);
        std::string responseString = response->toRawString();
        send(client->getFd(), responseString.c_str(), responseString.length(), 0);
        LOG_DEBUG("Socket " << client->getFd() << " sent");
        close(client->getFd());
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

std::vector<std::pair<const HttpRequest *, const Clients *> > PollManager::getRequests() {
	return _requests;
}

void PollManager::setResponses(std::vector<std::pair<const HttpResponse *, const Clients *> > responses) {
	for (std::vector<std::pair<const HttpResponse *, const Clients *> >::iterator it = responses.begin(); it != responses.end(); ++it) {
		_responses.push_back(*it);
	}
}

void PollManager::setRequestHandled(const HttpRequest *request) {
    for (std::vector<std::pair<const HttpRequest *, const Clients *> >::iterator it = _requests.begin(); it != _requests.end(); ++it) {
        if (it->first == request) {
            _requests.erase(it);
            return;
        }
    }
}

std::vector<std::pair<const HttpResponse *, const Clients *> > PollManager::getResponses() {
    return _responses;
}

