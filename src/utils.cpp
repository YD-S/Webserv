#include <fstream>
#include <vector>
#include "utils.hpp"
#include "PollManager/PollManager.hpp"
#include "Webserv.hpp"


bool is_dir(const std::string &path) {
	struct stat buf = {};
	stat(path.c_str(), &buf);
	return S_ISDIR(buf.st_mode);
}

std::string pathToFilename(const std::string &path) {
	size_t pos = path.find_last_of('/');
	if (pos == std::string::npos) {
		return path;
	}
	return path.substr(pos + 1);
}

int ft_stoi(const std::string &str) {
	std::istringstream iss(str);
	int result;

	if (!(iss >> result)) {
		// Handling the conversion error here
		throw std::invalid_argument("ft_stoi: Invalid input");
	}

	// Check for overflow or underflow
	if (iss.fail() || !iss.eof()) {
		throw std::out_of_range("ft_stoi: Out of range");
	}

	return result;
}

size_t ft_stoul(const std::string &str) {
	std::istringstream iss(str);
	size_t result;

	if (!(iss >> result)) {
		// Handling the conversion error here
		throw std::invalid_argument("ft_stoi: Invalid input");
	}

	// Check for overflow or underflow
	if (iss.fail() || !iss.eof()) {
		throw std::out_of_range("ft_stoi: Out of range");
	}

	return result;
}


void ft_error(unused const std::string &msg, int errorCode) {
	LOG_SYS_ERROR(msg);
	exit_handler(errorCode);
}

std::string ft_socket_to_string(struct sockaddr_in addr) {
	std::stringstream ss;
	ss << inet_ntoa(addr.sin_addr) << "::" << ntohs(addr.sin_port);
	return ss.str();
}

std::string intToInAddr(unsigned long ip) {
	struct in_addr addr;
	addr.s_addr = htonl(ip);
	return inet_ntoa(addr);
}

int fileExists(const std::string &path) {
	// If the file exists return 1
	// If the file does not exist return 0
	// If the file exists but is not accessible return -1
	struct stat buffer;
	int status = stat(path.c_str(), &buffer);
	if (status == 0) {
		if (S_ISREG(buffer.st_mode)) {
			return 1;
		} else {
			return -1;
		}
	} else {
		return 0;
	}
}

bool isBinaryFile(const std::string &path) {
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		LOG_SYS_ERROR("Error opening file: " << path);
		file.close();
	}
	char c;
	while (file.get(c)) {
		if (c == '\0') {
			file.close();
			return true;
		}
	}
	file.close();
	return false;
}

std::string trim(const std::string &str) {
	std::string::size_type first = str.find_first_not_of(' ');
	if (first == std::string::npos) {
		return "";
	}
	std::string::size_type last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

std::string stringToLower(const std::string &str) {
    std::string lowerStr = str;
    for (std::string::iterator it = lowerStr.begin(); it != lowerStr.end(); ++it) {
        *it = (char)tolower(*it);
    }
    return lowerStr;
}

void exit_handler(int status) {
	LOG_INFO("Stopping webserv...");
	for (unsigned long i = 0; i < serverSockets.size(); i++) {
		close(serverSockets[i].first);
		LOG_DEBUG("Socket " << serverSockets[i].first << " closed");
	}
    delete webserv;
	exit(status);
}