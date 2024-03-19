#include "macros.h"
#include "config/ServerConfig.hpp"
#include "Client/Client.hpp"
#include "Webserv.hpp"
#include <iostream>

std::vector<std::pair<int, const ServerConfig *> > serverSockets;
std::vector<Client> clients;

void ft_handle_sigint(unused int signal) {
	exit_handler(0);
}

int main(int argc, char *argv[], char **envp) {
	signal(SIGINT, ft_handle_sigint);
	signal(SIGTERM, ft_handle_sigint);
    webserv = new Webserv();
	LOG_INFO("Starting webserv...");
	if (argc == 1)
		webserv->parseConfig("configs/default.conf");
	else if (argc == 2)
		webserv->parseConfig(argv[1]);
	webserv->run(envp);
    delete webserv;
	return 0;
}
