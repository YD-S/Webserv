//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP


#include <string>
#include <vector>
#include <map>
#include "HttpRequest.hpp"
#include "config/ServerConfig.hpp"

class HttpResponse {
private:
    std::string version;
    int status;

    std::map<std::string, std::string> headers;

    std::string body;

public:

    HttpResponse();
    HttpResponse(const HttpResponse& other);
    HttpResponse& operator=(const HttpResponse& other);
    ~HttpResponse();

    HttpResponse setVersion(const std::string &version);

    HttpResponse setStatus(int status);

    HttpResponse addHeader(const std::string& key, const std::string& value);

    HttpResponse setBody(const std::string &body);

    const std::string &getVersion() const;

    int getStatus() const;

    const std::map<std::string, std::string> &getHeaders() const;

    const std::string &getBody() const;

    std::string toRawString();

    std::string toPrintableString();

    int findStatus(HttpRequest &request, ServerConfig &config);
};


#endif //WEBSERV_HTTPRESPONSE_HPP
