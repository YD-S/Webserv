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
    bool running;

public:
    Webserv();
    Webserv(const Webserv& other);
    Webserv& operator=(const Webserv& other);

    ~Webserv();

    void run();

    void stop();

    void parseConfig(std::string path);

    const std::string &getDateString();
};


#endif //WEBSERV_WEBSERV_HPP
