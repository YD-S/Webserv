#include <cstdlib>
#include "fileValidate/fileValidate.hpp"
#include "utils.hpp"

fileValidate::fileValidate() {}

fileValidate::fileValidate(const std::string &config_file) {
	this->_config_file = config_file;
}

fileValidate::fileValidate(const fileValidate& other) {
	*this = other;
}

fileValidate& fileValidate::operator=(const fileValidate& copy) {
	if (this != &copy) {
		this->_config_file = copy._config_file;
	}
	return *this;
}

fileValidate::~fileValidate() {}

void fileValidate::setConfigFile(const std::string &config_file) {
	this->_config_file = config_file;
}

std::string fileValidate::getConfigFile(){
	return this->_config_file;
}

std::ifstream fileValidate::validate_config_file(){
    if (is_dir(this->_config_file)) {
        LOG_ERROR("config file is a directory");
        exit (EXIT_FAILURE);
    }
	std::ifstream file(this->_config_file.c_str());
	if (!file.is_open()) {
		LOG_ERROR("Error opening config file: " << this->_config_file);
		file.close();
		exit (EXIT_FAILURE);
	}
	LOG_INFO("Config file: " << this->_config_file << " opened successfully");
	check_extension(this->_config_file);
	return (file);
}

void fileValidate::check_extension(std::string config_file){
	std::string extension;
	size_t pos = config_file.find_last_of('.');
	if (pos != std::string::npos) {
		extension = config_file.substr(pos + 1);
	}
	if (extension != "conf"){
		LOG_ERROR("Config file extension is not .conf");
		exit (EXIT_FAILURE);
	}
}