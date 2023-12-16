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
    std::vector<LocationConfig> _locations;
    LocationConfig _defaultLocation;

public:
    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

	void addListen(std::string host, int port);
    bool isListeningOn(int port);
	void removeListen(int port);
	void setServerName(const std::string& name);
	void addLocation(const LocationConfig& location);
	void setDefaultLocation(const LocationConfig& location);
	void setHostPort(const std::string host, int port);

    std::string getServerName();
	std::vector<LocationConfig>& getLocations();
	LocationConfig& getDefaultLocation();
	const std::vector<std::pair<std::string, int> >& getListen() const;
};


#endif //WEBSERV_SERVERCONFIG_HPP
