//
// Created by kolterdyx on 31/10/23.
//

#ifndef WEBSERV_BINCGIEXECUTOR_HPP
#define WEBSERV_BINCGIEXECUTOR_HPP


#include "ICgiExecutor.hpp"
#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "macros.h"
#include "utils.hpp"
#include "Webserv.hpp"
#include <sys/types.h>
#if __APPLE__
	#include <sys/event.h>
#elif __linux__
	#include <sys/epoll.h>
#endif

#define TIMEOUT_SEC 30

class BinCgiExecutor : public ICgiExecutor {

private:
	char **buildEnvp(HttpRequest *request);

	char **buildArgs(HttpRequest *request, const std::string& scriptPath);

	static void destroyCstrp(char **cstrp);

public:
	BinCgiExecutor();

	BinCgiExecutor(const std::string &cgiPath, const std::string &cgiName, char **envp, const LocationConfig *config);

	bool executeCgi(HttpRequest *request, std::string *response, std::string scriptPath);

	~BinCgiExecutor();

	static std::string convertToEnvVar(const std::string &header);

	std::string postProcessCgiResult();
};


#endif //WEBSERV_BINCGIEXECUTOR_HPP
