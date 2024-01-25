#include "macros.h"
#include "config/ServerConfig.hpp"
#include "Client/Client.hpp"
#include "Webserv.hpp"
#include <iostream>

std::vector<int> sockets;
std::vector<Client> clients;

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
    signal(SIGINT, ft_handle_sigint);
    Webserv webserv;
    LOG_INFO("Starting webserv...");
	if (argc == 1)
		webserv.parseConfig("configs/default.conf");
	else if (argc == 2)
        webserv.parseConfig(argv[1]);
    webserv.run();
	return 0;
}
