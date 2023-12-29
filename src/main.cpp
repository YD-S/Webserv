#include "macros.h"
#include "fileValidate/fileValidate.hpp"
#include "../includes/config/ServerConfig.hpp"
#include "../includes/config/LocationConfig.hpp"
#include "../includes/config/ParseConfig.hpp"
#include "PollManager/PollManager.hpp"
#include "Clients/Clients.hpp"
#include <iostream>
#include <string>

std::vector<int> sockets;
std::vector<Clients> clients;

void ft_handle_sigint(int signal) {
	(void)signal;
	LOG_INFO("Stopping webserv...");
	for (unsigned long i = 0; i < sockets.size(); i++) {
		close(sockets[i]);
		LOG_INFO("Socket " << sockets[i] << " closed");
	}
	exit(0);
}

int main(int argc, char *argv[]) {
    ParseConfig parse;
	PollManager PollManager;
	LOG_INFO("Starting webserv...");
	signal(SIGINT, ft_handle_sigint);
	if (argc == 1){
		parse = ParseConfig();
	}
	else if (argc == 2){
		std::string path = argv[1];
		parse = ParseConfig(path);
		
	}
	parse.parseConfig();
	if (parse.getServers().empty())
			LOG_ERROR("NO SERVER >:(");
	const std::vector<LocationConfig>& locations = (*(parse.getServers().begin())).getLocations();
	if (locations.empty())
			LOG_ERROR("NO LOCATIONS >:(");
	parse.printAll();
	PollManager.SocketConfig(parse.getServers());
	PollManager.Binder(parse.getServers());
	PollManager.Poller();
	return 0;
}
