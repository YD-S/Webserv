
#include "ServerConfig.hpp"

void ServerConfig::addListen(const std::string& host, int port) {
    listen.push_back(std::make_pair(host, port));
}

void ServerConfig::setServerName(const std::string& name) {
    serverName = name;
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

ServerConfig::ServerConfig() {

}

ServerConfig::~ServerConfig() {

}

void ServerConfig::removeListen(const std::string &host, int port) {
    for (std::vector<std::pair<std::string, int> >::iterator it = listen.begin(); it != listen.end(); ++it) {
        if (it->first == host && it->second == port) {
            listen.erase(it);
            return;
        }
    }
}
