//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP


#include <vector>
#include <string>
#include "LocationConfig.hpp"

class ServerConfig : public LocationConfig {

private:
    std::vector<std::pair<std::string, int> > listen;
    std::string serverName;

    std::vector<LocationConfig> locations;

public:
    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

    ServerConfig addListen(const std::string& host, int port);
    void removeListen(const std::string& host, int port);
    ServerConfig setServerName(const std::string& name);
    ServerConfig addLocation(const LocationConfig& location);


    bool isListeningOn(const std::string& host, int port);
    std::string getServerName();


};


#endif //WEBSERV_SERVERCONFIG_HPP
