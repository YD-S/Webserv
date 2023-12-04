//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_PARSECONFIG_HPP
#define WEBSERV_PARSECONFIG_HPP


#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <string>
#include <stack>
#include <unordered_map>
#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include "../fileValidate/fileValidate.hpp"
#include "macros.h"

class ParseConfig {

private:
    std::vector<ServerConfig> _servers;
    std::string _path;

public:

    ParseConfig();
    ParseConfig(const std::string& path);
    ParseConfig(const ParseConfig& other);
    ParseConfig& operator=(const ParseConfig& other);
    ~ParseConfig();

    bool isDelimiter(char c);
	void	validate_braces(std::ifstream &file);
	std::vector<std::pair<std::string, std::string> > processFile();
	void    parseConfig();
    void    mainSetter(std::unordered_multimap<std::string, std::string> &values);
    void addFunctionsServer(std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)> &functionMap);

    std::vector<ServerConfig>& getServers();
};


#endif //WEBSERV_PARSECONFIG_HPP
