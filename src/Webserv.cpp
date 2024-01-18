//
// Created by kolterdyx on 18/01/24.
//

#include "Webserv.hpp"

Webserv::Webserv() {
    LOG_INFO("Webserv created");
}

void Webserv::parseConfig(std::string path) {
    parse = ParseConfig(path);
	parse.parseConfig();
	if (parse.getServers().empty())
			LOG_ERROR("NO SERVER >:(");
	const std::vector<LocationConfig>& locations = (*(parse.getServers().begin())).getLocations();
	if (locations.empty())
			LOG_ERROR("NO LOCATIONS >:(");
	parse.printAll();
}

void Webserv::run() {
	pollManager.SocketConfig(parse.getServers());
	pollManager.Binder(parse.getServers());
	pollManager.Poller(parse.getServers());
}

Webserv::Webserv(const Webserv &other) {
    *this = other;
}

Webserv &Webserv::operator=(const Webserv &other) {
    if (this != &other) {
        this->parse = other.parse;
        this->pollManager = other.pollManager;
    }
    return *this;
}

Webserv::~Webserv() {
    LOG_DEBUG("Webserv destroyed");
}
