//
// Created by kolterdyx on 26/09/23.
//

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <string>
#include <sstream>
#include <sys/stat.h>
#include <cstdlib>
#include "macros.h"

bool is_dir(const std::string& path);

template<typename FuncType>
void processFunction(FuncType funcPointer) {
    // Call the function through the pointer
    funcPointer();
}

template <typename T>
std::string to_string(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}
int stoi(const std::string& str);
size_t stoul(const std::string& str);
std::string pathToFilename(const std::string& path);
void    ft_error(std::string msg, int errorCode);

#endif //WEBSERV_UTILS_HPP
