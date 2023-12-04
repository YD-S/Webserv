#include "macros.h"
#include "fileValidate/fileValidate.hpp"
#include "../includes/config/ServerConfig.hpp"
#include "../includes/config/LocationConfig.hpp"
#include "../includes/config/ParseConfig.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {

    fileValidate config_file;
    LOG_INFO("Starting webserv...");
	if (argc == 1){
		config_file.setConfigFile("config/default.conf");
	}
	else if (argc == 2){
		std::string path = argv[1];
		ParseConfig parse(path);
		parse.parseConfig();
		std::vector<ServerConfig>::iterator serverIter = parse.getServers().begin();
		for (; serverIter != parse.getServers().end(); ++serverIter){
			std::cout << serverIter->getHostName() << ":" << serverIter->getPort() 
			<< " - with default location at -> " << serverIter->getDefaultLocation().getPath() << std::endl;
		}
	}
	return 0;
}
