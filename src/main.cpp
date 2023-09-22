
#include "macros.h"
#include "fileValidate/fileValidate.hpp"
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
		config_file.setConfigFile(argv[1]);
	}
    LOG_DEBUG("Reading config file: " << config_file.getConfigFile());
	config_file.validate_config_file();
	return 0;
}