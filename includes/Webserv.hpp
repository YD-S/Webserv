//
// Created by kolterdyx on 18/01/24.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP


#include "config/ParseConfig.hpp"
#include "PollManager/PollManager.hpp"

class Webserv {
private:
    ParseConfig parse;
	PollManager pollManager;
//    bool running;

public:
    Webserv();
    Webserv(const Webserv& other);
    Webserv& operator=(const Webserv& other);

    ~Webserv();

    void run();

    void parseConfig(std::string path);

    HttpResponse * handleRequest(const HttpRequest *request, const ServerConfig *config);

    HttpResponse * handleWithLocation(const HttpRequest *request, const LocationConfig *config);

    const ServerConfig *getServerConfigByFd(int fd);
};


#endif //WEBSERV_WEBSERV_HPP
