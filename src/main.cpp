#include "macros.h"
#include "fileValidate/fileValidate.hpp"
#include "../includes/config/ServerConfig.hpp"
#include "../includes/config/LocationConfig.hpp"
#include "../includes/config/ParseConfig.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[]) {
    ParseConfig parse;
	LOG_INFO("Starting webserv...");
	if (argc == 1){
		parse = ParseConfig();
	}
	else if (argc == 2){
		std::string path = argv[1];
		parse = ParseConfig(path);
	}
	parse.parseConfig();
	parse.printAll();
	return 0;
}
