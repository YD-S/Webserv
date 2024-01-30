//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_SERVERCONFIG_HPP
#define WEBSERV_SERVERCONFIG_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <algorithm>
#include <csignal>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include <cstdio>
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
	void addLocation(LocationConfig location);
	void setDefaultLocation(const LocationConfig& location);
	void setHostPort(const std::string host, int port);

    std::string getServerName() const;
	const std::vector<LocationConfig>& getLocations() const;
	const LocationConfig * getDefaultLocation() const;
	const std::vector<std::pair<std::string, int> >& getListen() const;
};


#endif //WEBSERV_SERVERCONFIG_HPP
