//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_HTTPRESPONSE_HPP
#define WEBSERV_HTTPRESPONSE_HPP


#include <string>
#include <vector>

class HttpResponse {
private:
    std::string version;
    int status;
    std::string reason;

    std::vector<std::pair<std::string, std::string> > headers;

    std::string body;

public:

    HttpResponse();
    ~HttpResponse();

    void setVersion(const std::string &version);

    void setStatus(int status);

    void addHeader(const std::string& key, const std::string& value);

    void setBody(const std::string &body);

    const std::string &getVersion() const;

    int getStatus() const;

    const std::string &getReason() const;

    const std::vector<std::pair<std::string, std::string> > &getHeaders() const;

    const std::string &getBody() const;

    std::string toRawString();

    std::string toPrintableString();
};


#endif //WEBSERV_HTTPRESPONSE_HPP
