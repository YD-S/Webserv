//
// Created by kolterdyx on 26/09/23.
//

#ifndef WEBSERV_UTILS_HPP
#define WEBSERV_UTILS_HPP

#include <string>
#include <sstream>

bool is_dir(const std::string& path);

template <typename T>
std::string to_string(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string pathToFilename(const std::string& path);

#endif //WEBSERV_UTILS_HPP
