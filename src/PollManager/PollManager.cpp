//
// Created by Yash on 19/12/23.
//
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include "PollManager/PollManager.hpp"

PollManager::PollManager() : clientSocket(), clientData() {
}

PollManager::~PollManager() {
}

PollManager::PollManager(const PollManager &src) {
    *this = src;
}

PollManager &PollManager::operator=(const PollManager &src) {
    if (this != &src) {
        this->_servers = src._servers;
    }
    return *this;
}

void PollManager::socketConfig(const std::vector<ServerConfig> &serversConfig) {
    for (unsigned long i = 0; i < serversConfig.size(); ++i) {
		for (unsigned long j = 0; j < serversConfig[i].getListen().size(); j++) {
			int s = socket(AF_INET, SOCK_STREAM, 0);
			if (s < 0) {
				LOG_SYS_ERROR("Socket creation failed");
				kill(getpid(), SIGINT);
			}
			int reuse = 1;
			setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *) &reuse, sizeof(reuse)); // Reuse the port
			struct timeval timeout;
			timeout.tv_sec = 0; // 5 seconds timeout
			timeout.tv_usec = 30000; // Make it only for POST requests as it affects the benchmark
			setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *) &timeout, sizeof(timeout)); // timeout for recv
			serverSockets.push_back(std::make_pair(s, &serversConfig[i]));
		}
    }
    for (unsigned long i = 0; i < serversConfig.size(); ++i) {
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        for (unsigned long j = 0; j < serversConfig[i].getListen().size(); j++) {
            if (serversConfig[i].getListen()[j].first == "localhost")
                server.sin_addr.s_addr = inet_addr("127.0.0.1");
            else if (serversConfig[i].getListen()[j].first == "0.0.0.0")
                server.sin_addr.s_addr = INADDR_ANY;
            else
                server.sin_addr.s_addr = inet_addr(serversConfig[i].getListen()[j].first.c_str());
            server.sin_port = htons(serversConfig[i].getListen()[j].second);
			_servers.push_back(server);
		}
    }
}

void PollManager::binder(const std::vector<ServerConfig> &servers) {
    LOG_DEBUG("Binding " << _servers.size() << " servers");
    for (unsigned long i = 0; i < _servers.size(); i++) {
        if (bind(serverSockets[i].first, (struct sockaddr *) &_servers[i], sizeof(struct sockaddr_in)) == -1) {
            LOG_SYS_ERROR("Bind failed");
            kill(getpid(), SIGINT);
        }
    }
    for (unsigned long i = 0; i < servers.size(); i++)
		for (unsigned long j = 0; j < servers[i].getListen().size(); j++)

        	LOG_INFO("Socket " << serverSockets[i].first << " is bound to " << servers[i].getListen()[j].first << ":"
                           	<< servers[i].getListen()[j].second);
    for (unsigned long i = 0; i < serverSockets.size(); i++) {
        if (listen(serverSockets[i].first, 25) < 0) {
            LOG_SYS_ERROR("Listen failed");
            kill(getpid(), SIGINT);
        }

    }
}

void PollManager::poller() {
    fd_set write_fd;
    fd_set read_fd;
    fd_set error_fd;
    FD_ZERO(&write_fd);
    FD_ZERO(&read_fd);
    FD_ZERO(&error_fd);
    int max_fd = 0;

    // Server sockets for reading and error checking
    FILL_SET(serverSockets, serverSockets[i].first, read_fd, max_fd);
    FILL_SET(serverSockets, serverSockets[i].first, error_fd, max_fd);

    // Client sockets for writing and error checking
    FILL_SET(clients, clients[i].getFd(), write_fd, max_fd);
    FILL_SET(clients, clients[i].getFd(), error_fd, max_fd);

    timeval timeout = {0, 0};
    if (select(max_fd + 1, &read_fd, &write_fd, &error_fd, &timeout) < 0) {
        LOG_SYS_ERROR("Select failed");
    }
    for (unsigned long i = 0; i < serverSockets.size(); i++) {
        if (FD_ISSET(serverSockets[i].first, &read_fd)) {
            socklen_t sizeClient = sizeof(clientData);
			if(_requests.size() < 256)
            	clientSocket = accept(serverSockets[i].first, (struct sockaddr *) &clientData, &sizeClient);
            if (clientSocket < 0) {
                LOG_SYS_ERROR("Accept failed");
                continue;
            }
            LOG_DEBUG("Socket " << clientSocket << " accepted");

            std::string requestString;
            char buffer[BUFFER_SIZE] = {0};
            int readValue = 0;
            do {
                for (int j = 0; j < BUFFER_SIZE; j++)
                    buffer[j] = 0;
                readValue = read(clientSocket, buffer, BUFFER_SIZE);
                if (readValue < 0) {
                    LOG_SYS_ERROR("Read failed");
                    continue;
                }
                requestString.append(buffer, readValue);
            } while (readValue == BUFFER_SIZE);
            Client client = Client(clientSocket, clientData);
            clients.push_back(client);
            HttpRequest *request = new HttpRequest();
            request->setFd(serverSockets[i].first);
            request->parse(requestString);
            _requests.push_back(std::make_pair(request, client));

            LOG_DEBUG("Request received from socket " << clientSocket);
        }
    }

    std::vector<const HttpResponse *> responsesSent;
    for (unsigned long i = 0; i < _responses.size(); i++) {
        HttpResponse *response = _responses[i].first;
        if (!response)
            continue;
        Client *client = &_responses[i].second;
        if (!FD_ISSET(client->getFd(), &write_fd))
            continue;
        std::string responseString = response->toRawString();
        send(client->getFd(), responseString.c_str(), responseString.length(), 0);
        LOG_DEBUG("Response sent to socket " << client->getFd());
        responsesSent.push_back(response);
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
                LOG_DEBUG("Deleting response");
                delete responsesSent[i];
                break;
            }
        }
    }
}

std::vector<std::pair<HttpRequest *, Client> > PollManager::getRequests() {
    return _requests;
}

void PollManager::setResponses(std::vector<std::pair<HttpResponse *, Client> > responses) {
    for (std::vector<std::pair<HttpResponse *, Client> >::iterator it = responses.begin();
         it != responses.end(); ++it) {
        _responses.push_back(std::make_pair(it->first, it->second));
    }
}

void PollManager::setRequestHandled(HttpRequest *request) {
    for (std::vector<std::pair<HttpRequest *, Client> >::iterator it = _requests.begin(); it != _requests.end(); ++it) {
        if (it->first == request) {
            delete it->first;
            _requests.erase(it);
            return;
        } else
            LOG_SYS_ERROR("Request not found");
    }
}

std::vector<std::pair<HttpResponse *, Client> > PollManager::getResponses() {
    return _responses;
}

