#include "../includes/config/ServerConfig.hpp"

ServerConfig::ServerConfig() {

}

ServerConfig::ServerConfig(const ServerConfig &other) {
    *this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        this->_listen.clear();
		for (std::vector<std::pair<std::string,int> >::const_iterator it = other._listen.begin(); it != other._listen.end(); ++it) {
            this->_listen.push_back(std::make_pair(it->first, it->second));
        }
        this->_serverName = other._serverName;
		this->_errorPages = other._errorPages;
		this->_clientMaxBodySize = other._clientMaxBodySize;
		this->_defaultLocation = other._defaultLocation;
        this->_locations.clear();
        for (std::vector<LocationConfig>::const_iterator it = other._locations.begin();
             it != other._locations.end(); ++it) {
            this->_locations.push_back(LocationConfig(*it));
        }
    }
    return *this;
}

ServerConfig::~ServerConfig() {

}

void ServerConfig::addListen(std::string host, int port){
	_listen.push_back(std::make_pair(host, port));
}

void ServerConfig::setServerName(const std::string &name) {
	_serverName = name;
}

bool ServerConfig::isListeningOn(int port) {
	for (std::vector<std::pair<std::string,int> >::const_iterator it = _listen.begin(); it != _listen.end(); ++it) {
            if (it->second == port)
				return true;
    }
	return false;
}

std::string ServerConfig::getServerName() {
    return _serverName;
}

void ServerConfig::removeListen(int port) {
    std::vector<std::pair<std::string,int> >::const_iterator it = _listen.end();
    for (std::vector<std::pair<std::string,int> >::const_iterator iter = _listen.begin(); iter != _listen.end(); ++iter) {
            if (iter->second == port)
			{
				it = iter;
				break ;
			}
        }
	//if (it != _listen.end())
	//	_listen.erase(it);
}

void ServerConfig::addLocation(const LocationConfig &location) {
    this->_locations.push_back(location);
}

void ServerConfig::setDefaultLocation(const LocationConfig &location) {
    this->_defaultLocation = location;
}

void ServerConfig::setClientMaxBodySize(size_t size){
	_clientMaxBodySize = size;
}
void ServerConfig::setClientMaxBodySize(const std::string& size){
	try {
		_clientMaxBodySize = stoi(size);
	} catch (std::exception &e) { std::runtime_error("clientMaxBodySize too large!"); }
}

void ServerConfig::setErrorPages(const std::string& errorPages){
	_errorPages = errorPages;
}

std::vector<LocationConfig>& ServerConfig::getLocations(){
	return _locations;
}

LocationConfig& ServerConfig::getDefaultLocation(){
	return _defaultLocation;
}