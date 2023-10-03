
#ifndef WEBSERV_FILEVALIDATE_HPP
#define WEBSERV_FILEVALIDATE_HPP
#include "HttpRequest.hpp"
#include "macros.h"
#include <iostream>
#include <fstream>
#include <string>

class fileValidate {
private:
	std::string _config_file;
public:
	fileValidate();
	fileValidate(const std::string &config_file);
	fileValidate(const fileValidate& other);
	fileValidate& operator=(const fileValidate& other);
	~fileValidate();
	void setConfigFile(const std::string &config_file);
	std::string getConfigFile();
	void validate_config_file();
	static void check_extension(std::string config_file);

};

#endif
