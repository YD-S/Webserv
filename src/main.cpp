#include "macros.h"
#include "fileValidate/fileValidate.hpp"
#include "../includes/config/ServerConfig.hpp"
#include "../includes/config/LocationConfig.hpp"
#include "../includes/config/ParseConfig.hpp"
#include "PollManager/PollManager.hpp"
#include "Clients/Clients.hpp"
#include "Webserv.hpp"
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
