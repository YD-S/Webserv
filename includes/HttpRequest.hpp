#ifndef WEBSERV_HTTPREQUEST_HPP
#define WEBSERV_HTTPREQUEST_HPP


#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include "utils.hpp"
#include "macros.h"
#include "../includes/config/LocationConfig.hpp"

class HttpRequest {

private:
    std::string _method;
    std::string _path;
    std::string _version;

    std::map<std::string, std::string> _params;
    std::map<std::string, std::string> _headers;

    std::string _body;

    int _serverFd;
public:

    HttpRequest();

    HttpRequest(const HttpRequest &other);

    HttpRequest &operator=(const HttpRequest &other);

    ~HttpRequest();

    HttpRequest *setMethod(const std::string &method);

    HttpRequest *setPath(const std::string &path);

    HttpRequest *setVersion(const std::string &versionM);

    HttpRequest *setHeader(const std::string &key, const std::string &value);

    HttpRequest *addParam(const std::string &key, const std::string &value);

    const HttpRequest *setBody(const std::string &body);

    const std::string &getMethod() const;

    const std::string &getPath() const;

    const std::string &getVersion() const;

    const std::map<std::string, std::string> &getHeaders() const;

    std::string getHeader(const std::string &key) const;

    const std::map<std::string, std::string> &getParams() const;

    std::string getParam(const std::string &key) const;

    std::string getBody() const;

    std::string toRawString() const;

    std::string toPrintableString() const;

    std::string getQueryString() const;

    void parseGetParams(const std::string &path_string);

    void parsePostParams(std::istringstream &stream);

    void printHttpRequest() const;

    int getFd() const;

    void setFd(int fd);

    static std::string trim(const std::string &str);

    int isInvalid(const LocationConfig *location) const;

    HttpRequest *parseHeader(std::string &request);

    HttpRequest *parseBody(std::string &request);
};


#endif //WEBSERV_HTTPREQUEST_HPP
