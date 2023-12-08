//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <stack>
#include "LocationConfig.hpp"
#include "macros.h"

class ServerConfig {

private:
	std::vector<std::pair<std::string, int> > _listen;
	
	std::string _serverName;
	std::string _errorPages;
	size_t _clientMaxBodySize;
    std::vector<LocationConfig> _locations;
    LocationConfig _defaultLocation;

public:
    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

	void addListen(std::string host, int port);
	//void setHost(const std::string& host);
    bool isListeningOn(int port);
	void removeListen(int port);
	void setServerName(const std::string& name);
	void addLocation(const LocationConfig& location);
	void setDefaultLocation(const LocationConfig& location);
	void setClientMaxBodySize(size_t size);
	void setClientMaxBodySize(const std::string& size);
	void setErrorPages(const std::string& errorPages);


    std::string getServerName();
	//std::string getHostName();
	//int getPort();
	std::vector<LocationConfig>& getLocations();
	LocationConfig& getDefaultLocation();
};


#endif //WEBSERV_SERVERCONFIG_HPP
