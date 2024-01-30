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
#include "LocationConfig.hpp"
#include "ServerConfig.hpp"
#include "../fileValidate/fileValidate.hpp"
#include "../utils.hpp"
#include "macros.h"

class ParseConfig {

private:
	std::vector<ServerConfig> _servers;
	std::string _path;

public:

	ParseConfig();

	ParseConfig(const std::string &path);

	ParseConfig(const ParseConfig &other);

	ParseConfig &operator=(const ParseConfig &other);

	~ParseConfig();

	bool isDelimiter(char c);

	LocationConfig parseLocation(std::vector<std::pair<std::string, std::string> >::iterator &it,
								 std::vector<std::pair<std::string, std::string> > &config);

	void validate_braces(std::ifstream &file);

	std::vector<std::pair<std::string, std::string> > processFile();

	void parseConfig();

	void mainSetter(std::vector<std::pair<std::string, std::vector<std::string> > > &variables, ServerConfig &server);

	void LocationSetter(std::vector<std::pair<std::string, std::vector<std::string> > > &variables,
						LocationConfig &location);

	void printTempVariables(std::vector<std::pair<std::string, std::vector<std::string> > > variables);

	std::string extractAndRemoveFirst(std::vector<std::string> &values);

	void separateHostPort(ServerConfig &server, const std::string &input);

	void printLocation(const LocationConfig &location);

	void printAll();

	std::vector<ServerConfig> &getServers();
};


#endif //WEBSERV_PARSECONFIG_HPP
