//
// Created by kolterdyx on 13/09/23.
//

#ifndef WEBSERV_MACROS_H
#define WEBSERV_MACROS_H

#include <iostream>
#include <csignal>
#include <cstring>
#include <cerrno>

#define unused __attribute__((unused))


// Colors
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

#ifndef DEBUG
    #define DEBUG 3
#endif

#define ERRNO ()

// Log
#if DEBUG <= 3
    #define LOG_SYS_ERROR(msg) std::cerr << RED << "[ERROR] " << msg << ": " << errno << " " << strerror(errno) << RESET << std::endl
#else
    #define LOG_SYS_ERROR(msg)
#endif

#if DEBUG <= 2
    #define LOG_WARNING(msg) std::cerr << YELLOW << "[WARNING] " << msg << RESET << std::endl
#else
    #define LOG_WARNING(msg)
#endif

#if DEBUG <= 1
    #define LOG_INFO(msg) std::cout << GREEN << "[INFO] " << msg << RESET << std::endl
#else
    #define LOG_INFO(msg)
#endif

#if DEBUG <= 0
    #define LOG_DEBUG(msg) std::cout << WHITE << "[DEBUG] " << msg << RESET << std::endl
#else
    #define LOG_DEBUG(msg)
#endif


#endif //WEBSERV_MACROS_H
