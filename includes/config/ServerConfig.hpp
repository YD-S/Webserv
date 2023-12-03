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
#include <unordered_map>
#include "LocationConfig.hpp"
#include "macros.h"

class ServerConfig {

private:
    std::vector<std::pair<std::string, int> > _listen;
    std::string _serverName;
	std::string _host;
	size_t _clientMaxBodySize;

    std::vector<LocationConfig> _locations;
    LocationConfig _defaultLocation;

public:
    ServerConfig();
    ServerConfig(const ServerConfig& other);
    ServerConfig& operator=(const ServerConfig& other);
    ~ServerConfig();

	bool isDelimiter(char c);
	void	validate_braces(std::ifstream &file);
	std::vector<std::pair<std::string, std::string> > processFile(const std::string& filename);
	std::vector<ServerConfig>    parseConfig(std::string &path);
    void    mainSetter(std::unordered_multimap<std::string, std::string> &values);
	void getLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config);

	void addListen(const std::string& host, int port);
    void removeListen(const std::string& host, int port);
	void setServerName(const std::string& name);
	void addLocation(const LocationConfig& location);
	void setDefaultLocation(const LocationConfig& location);
	void setClientMaxBodySize(size_t size);
	void setClientMaxBodySize(const std::string& size);

    bool isListeningOn(const std::string& host, int port);
    std::string getServerName();

};


#endif //WEBSERV_SERVERCONFIG_HPP
