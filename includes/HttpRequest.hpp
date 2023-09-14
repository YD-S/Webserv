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

public:

    HttpRequest();
    HttpRequest(const HttpRequest& other);
    HttpRequest& operator=(const HttpRequest& other);
    ~HttpRequest();

    HttpRequest setMethod(const std::string &method);

    HttpRequest setPath(const std::string &path);

    HttpRequest setVersion(const std::string &versionM);

    HttpRequest addHeader(const std::string &key, const std::string &value);

    const HttpRequest &setBody(const std::string &body);

    const std::string &getMethod() const;

    const std::string &getPath() const;

    const std::string &getVersion() const;

    const std::vector<std::pair<std::string, std::string> > &getHeaders() const;

    const std::string &getBody() const;

    std::string toRawString();

    std::string toPrintableString();
};


#endif //WEBSERV_HTTPREQUEST_HPP
