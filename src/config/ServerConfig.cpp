#include "../includes/config/ServerConfig.hpp"

ServerConfig::ServerConfig() {

}

ServerConfig::ServerConfig(const ServerConfig &other) {
    *this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        this->_listen = other._listen;
		this->_host = other._host;
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

void ServerConfig::setLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config){
	LocationConfig myLocation;
	myLocation.parseLocation(it, config);
	addLocation(myLocation);
}

void ServerConfig::addListen(int port){
	_listen = port;
}
void ServerConfig::addListen(const std::string& port){
	try {
		_listen = std::stoi(port);
	} catch (std::exception &e) { LOG_ERROR(e.what()); }
}

void ServerConfig::setServerName(const std::string &name) {
	_serverName = name;
}

bool ServerConfig::isListeningOn(int port) {
    return _listen == port;
}

std::string ServerConfig::getServerName() {
    return _serverName;
}

void ServerConfig::removeListen() {
    _listen = -1;
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
		_clientMaxBodySize = std::stol(size);
	} catch (std::exception &e) { std::runtime_error("clientMaxBodySize too large!"); }
}

void ServerConfig::setHost(const std::string& host){
	_host = host;
}

void ServerConfig::setErrorPages(const std::string& errorPages){
	_errorPages = errorPages;
}

std::string ServerConfig::getHostName(){
	return _host;
}
int ServerConfig::getPort(){
	return _listen;
}

std::vector<LocationConfig>& ServerConfig::getLocations(){
	return _locations;
}

LocationConfig& ServerConfig::getDefaultLocation(){
	return _defaultLocation;
}