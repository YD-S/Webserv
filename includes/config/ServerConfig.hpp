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
#include <unordered_map>
#include "LocationConfig.hpp"
#include "macros.h"

class ServerConfig {

private:
    int _listen;
	std::string _host;
	
	std::string _errorPages;
	std::string _serverName;
	size_t _clientMaxBodySize;
    std::vector<LocationConfig> _locations;
    LocationConfig _defaultLocation;

public:
    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

	void setLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config);

	void addListen(int port);
	void addListen(const std::string& port);
	void setHost(const std::string& host);
    void removeListen();
	void setServerName(const std::string& name);
	void addLocation(const LocationConfig& location);
	void setDefaultLocation(const LocationConfig& location);
	void setClientMaxBodySize(size_t size);
	void setClientMaxBodySize(const std::string& size);
	void setErrorPages(const std::string& errorPages);

    bool isListeningOn(int port);
    std::string getServerName();
	std::string getHostName();
	int getPort();
	std::vector<LocationConfig>& getLocations();
	LocationConfig& getDefaultLocation();
};


#endif //WEBSERV_SERVERCONFIG_HPP
