#include "utils.hpp"


bool is_dir(const std::string& path) {
    struct stat buf = {};
    stat(path.c_str(), &buf);
    return S_ISDIR(buf.st_mode);
}

std::string pathToFilename(const std::string& path) {
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return path;
    }
    return path.substr(pos + 1);
}

int stoi(const std::string& str) {
    std::istringstream iss(str);
    int result;
    
    if (!(iss >> result)) {
        // Handling the conversion error here
        throw std::invalid_argument("stoi: Invalid input");
    }

    // Check for overflow or underflow
    if (iss.fail() || !iss.eof()) {
        throw std::out_of_range("stoi: Out of range");
    }
    
    return result;
}

size_t stoul(const std::string& str) {
    std::istringstream iss(str);
    size_t result;
    
    if (!(iss >> result)) {
        // Handling the conversion error here
        throw std::invalid_argument("stoi: Invalid input");
    }

    // Check for overflow or underflow
    if (iss.fail() || !iss.eof()) {
        throw std::out_of_range("stoi: Out of range");
    }
    
    return result;
}


void    ft_error(std::string msg, int errorCode)
{
	LOG_ERROR(msg);
	exit(errorCode);
}
