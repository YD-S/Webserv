#include "macros.h"
#include "config/ServerConfig.hpp"
#include "Client/Client.hpp"
#include "Webserv.hpp"
#include <iostream>

std::vector<std::pair<int, const ServerConfig *> > serverSockets;
std::vector<Client> clients;

void ft_handle_sigint(unused int signal) {
	exit_handler();
}

int main(int argc, char *argv[], char **envp) {
	signal(SIGINT, ft_handle_sigint);
	signal(SIGTERM, ft_handle_sigint);
	Webserv webserv;
	LOG_INFO("Starting webserv...");
	if (argc == 1)
		webserv.parseConfig("configs/default.conf");
	else if (argc == 2)
		webserv.parseConfig(argv[1]);
	webserv.run(envp);
	return 0;
}
