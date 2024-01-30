//
// Created by kolterdyx on 26/09/23.
//

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <cstdlib>
#include "macros.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

bool is_dir(const std::string &path);

template<typename FuncType>
void processFunction(FuncType funcPointer) {
	// Call the function through the pointer
	funcPointer();
}

template<typename T>
std::string to_string(T value) {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

int ft_stoi(const std::string &str);

size_t ft_stoul(const std::string &str);

std::string pathToFilename(const std::string &path);

void ft_error(std::string msg, int errorCode);

std::string ft_socket_to_string(struct sockaddr_in addr);

std::string intToInAddr(unsigned long ip);

int fileExists(std::string path);

bool isBinaryFile(const std::string& path);

#endif //WEBSERV_UTILS_HPP
