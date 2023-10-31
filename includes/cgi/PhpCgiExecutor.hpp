//
// Created by kolterdyx on 31/10/23.
//

#ifndef WEBSERV_PHPCGIEXECUTOR_HPP
#define WEBSERV_PHPCGIEXECUTOR_HPP


#include "ICgiExecutor.hpp"

class PhpCgiExecutor : public ICgiExecutor {

private:
    char **buildEnvp(HttpRequest &request);

    char **buildArgs(HttpRequest &request);

    static void destroyCstrp(char **cstrp);


public:
    PhpCgiExecutor();

    PhpCgiExecutor(const std::string &cgiPath, const std::string &cgiName);

    void executeCgi(HttpRequest &request, HttpResponse &response);

    ~PhpCgiExecutor();

    static std::string convertToEnvVar(const std::string &header);
};


#endif //WEBSERV_PHPCGIEXECUTOR_HPP
