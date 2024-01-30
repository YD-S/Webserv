//
// Created by kolterdyx on 18/01/24.
//

#ifndef WEBSERV_WEBSERV_HPP
#define WEBSERV_WEBSERV_HPP


#include "config/ParseConfig.hpp"
#include "PollManager/PollManager.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class Webserv {
private:
	ParseConfig parse;

	PollManager pollManager;

	HttpResponse *handleRequest(const HttpRequest *request, const ServerConfig *config);

	HttpResponse *handleWithLocation(const HttpRequest *request, const LocationConfig *config);

	static const ServerConfig *getServerConfigByFd(int fd);

	static void setErrorResponse(HttpResponse *response, const int statusCode, LocationConfig *config);

	bool checkRequestMethod(const HttpRequest *request, const LocationConfig *config, HttpResponse *response);

	static void setDefaultResponse(HttpResponse *response, LocationConfig *config);

	HttpResponse *generateAutoIndex(const HttpRequest *request, const LocationConfig *config);

	HttpResponse *getFile(const HttpRequest *request, const LocationConfig *config, HttpResponse *response);

	static std::string getDirPath(const HttpRequest *request, const LocationConfig *config) ;

public:
	Webserv();

	Webserv(const Webserv &other);

	Webserv &operator=(const Webserv &other);

	~Webserv();

	void run();

	void parseConfig(const std::string& path);

	HttpResponse * getIndex(HttpRequest *request, const LocationConfig *config, HttpResponse *response);
};


#endif //WEBSERV_WEBSERV_HPP
