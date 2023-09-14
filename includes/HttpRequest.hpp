//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_HTTPREQUEST_HPP
#define WEBSERV_HTTPREQUEST_HPP


#include <string>
#include <vector>

class HttpRequest {

private:
    std::string method;
    std::string path;
    std::string version;

    std::vector<std::pair<std::string, std::string> > headers;

    std::string body;
};


#endif //WEBSERV_HTTPREQUEST_HPP
