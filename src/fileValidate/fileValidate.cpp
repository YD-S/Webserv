#include "fileValidate/fileValidate.hpp"

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

void fileValidate::validate_config_file(){
	std::ifstream file(this->_config_file);
	if (!file.is_open()) {
		LOG_ERROR("Error opening config file: " << this->_config_file);
		file.close();
		exit (EXIT_FAILURE);
	}
	LOG_INFO("Config file: " << this->_config_file << " opened successfully");
	file.close();
}
