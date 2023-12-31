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

class HttpRequest {

private:
    std::string _method;
    std::string _path;
    std::string _version;

    std::map<std::string, std::string> _params;
    std::map<std::string, std::string> _headers;

    std::string _body;

public:

    HttpRequest();
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    HttpRequest parse(std::string request);

    HttpRequest setMethod(const std::string &method);

    HttpRequest setPath(const std::string &path);

    HttpRequest setVersion(const std::string &versionM);

    HttpRequest addHeader(const std::string &key, const std::string &value);

    HttpRequest addParam(const std::string &key, const std::string &value);

    const HttpRequest &setBody(const std::string &body);

    const std::string &getMethod() const;

    const std::string &getPath() const;

    const std::string &getVersion() const;

    const std::map<std::string, std::string> &getHeaders() const;

    const std::string &getHeader(const std::string &key) const;

    const std::map<std::string, std::string> &getParams() const;

    const std::string &getParam(const std::string &key) const;

    const std::string &getBody() const;

    std::string toRawString();

    std::string toPrintableString();

    std::string getQueryString();
    
    void parseGetParams(const std::string &path_string);
    
    void parsePostParams(std::istringstream &stream);

    void printHttpRequest() const;
};


#endif //WEBSERV_HTTPREQUEST_HPP
