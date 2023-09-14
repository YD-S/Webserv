
#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {

}

ServerConfig::ServerConfig(const ServerConfig &other)  : LocationConfig(other) {
    *this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        this->listen = other.listen;
        this->serverName = other.serverName;
        this->locations.clear();
        for (std::vector<LocationConfig>::const_iterator it = other.locations.begin();
             it != other.locations.end(); ++it) {
            this->locations.push_back(LocationConfig(*it));
        }
    }
    return *this;
}

ServerConfig::~ServerConfig() {

}

ServerConfig ServerConfig::addListen(const std::string &host, int port) {
    listen.push_back(std::make_pair(host, port));
    return *this;
}

ServerConfig ServerConfig::setServerName(const std::string &name) {
    serverName = name;
    return *this;
}

bool ServerConfig::isListeningOn(const std::string &host, int port) {
    for (std::vector<std::pair<std::string, int> >::iterator it = listen.begin(); it != listen.end(); ++it) {
        if (it->first == host && it->second == port) {
            return true;
        }
    }
    return false;
}

std::string ServerConfig::getServerName() {
    return serverName;
}

void ServerConfig::removeListen(const std::string &host, int port) {
    for (std::vector<std::pair<std::string, int> >::iterator it = listen.begin(); it != listen.end(); ++it) {
        if (it->first == host && it->second == port) {
            listen.erase(it);
            return;
        }
    }
}

ServerConfig ServerConfig::addLocation(const LocationConfig &location) {
    this->locations.push_back(location);
    return *this;
}

ServerConfig ServerConfig::setDefaultLocation(const LocationConfig &location) {
    this->defaultLocation = location;
}
