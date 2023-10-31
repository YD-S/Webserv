//
// Created by kolterdyx on 31/10/23.
//

#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "cgi/PhpCgiExecutor.hpp"
#include "macros.h"
#include "utils.hpp"

void PhpCgiExecutor::executeCgi(HttpRequest &request, HttpResponse &response) {

    // Create a pipe to communicate with the CGI
    // We need to write the request body to the pipe and read the response from the pipe,
    // so we need two file descriptors
    int request_pipe[2];
    int response_pipe[2];
    if (pipe(request_pipe) == -1 || pipe(response_pipe) == -1) {
        throw std::runtime_error("Failed to create pipe");
    }

    // Build the environment variables
    char **envp = buildEnvp(request);

    // Build the arguments
    char **args = buildArgs(request);

    // Fork the process
    pid_t pid = fork();
    if (pid == -1) {
        throw std::runtime_error("Failed to fork");
    }
    // Child process
    if (pid == 0) {
        // Duplicate the write end of the response_pipe to stdout
        if (dup2(response_pipe[1], STDOUT_FILENO) == -1) {
            throw std::runtime_error("Failed to duplicate STDOUT_FILENO pipe");
        }

        // Duplicate the read end of the request_pipe to stdin
        if (dup2(request_pipe[0], STDIN_FILENO) == -1) {
            throw std::runtime_error("Failed to duplicate STDIN_FILENO pipe");
        }

        // Execute the CGI
        execve(this->getCgiPath().c_str(), args, envp);

        // If we get here, execve failed
        throw std::runtime_error("Failed to execute php-cgi");
    }
    // Parent process
    else {
        // Write the request body to the pipe
        if (write(request_pipe[1], request.getBody().c_str(), request.getBody().size()) == -1) {
            throw std::runtime_error("Failed to write to pipe");
        }

        // Close the write end of the pipe
        close(response_pipe[1]);

        // Read the response from the CGI
        char buffer[1024];
        _cgiResult = "";
        ssize_t bytes_read;
        while ((bytes_read = read(response_pipe[0], buffer, 1024)) > 0) {
            _cgiResult += std::string(buffer, bytes_read);
        }

        // Close the read end of the pipe
        close(response_pipe[0]);

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        destroyCstrp(envp);
        destroyCstrp(args);

        // Set the response body
        response.setBody(_cgiResult);
    }


}

char **PhpCgiExecutor::buildEnvp(HttpRequest &request) {
    // Build the environment variables
    std::vector<std::string> envp_vector = std::vector<std::string>();
    envp_vector.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envp_vector.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envp_vector.push_back("REDIRECT_STATUS=200");
    envp_vector.push_back("REQUEST_METHOD=" + request.getMethod());
    envp_vector.push_back("SCRIPT_FILENAME=" + request.getPath());
    envp_vector.push_back("BODY=" + request.getBody());
    // Add headers to the environment variables
    for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin();
         it != request.getHeaders().end(); ++it) {
        envp_vector.push_back(convertToEnvVar(it->first) + "=" + it->second);
    }
    // Check required headers like Content-Length and add them if they are missing
    if (request.getHeaders().find("Content-Length") == request.getHeaders().end()) {
        envp_vector.push_back("CONTENT_LENGTH=" + to_string(request.getBody().size()));
    }

    // Add the system environment variables to the environment pointer array
    for (char **env = __environ; *env != NULL; ++env) {
        envp_vector.push_back(*env);
    }

    // Add the environment variables to the environment pointer array
    char **envp = new char *[envp_vector.size() + 1];
    for (std::vector<std::string>::size_type i = 0; i < envp_vector.size(); ++i) {
        envp[i] = new char[envp_vector[i].size() + 1];
        std::strcpy(envp[i], envp_vector[i].c_str());
    }
    envp[envp_vector.size()] = NULL;
    return envp;
}

void PhpCgiExecutor::destroyCstrp(char **cstrp) {
    for (char **str = cstrp; *str != NULL; ++str) {
        delete[] *str;
    }
    delete[] cstrp;
}

char **PhpCgiExecutor::buildArgs(unused HttpRequest &request) {
    // The first argument (omitting the argv[0] executable name) is the file requested
    std::vector<std::string> args_vector = std::vector<std::string>();
    args_vector.push_back(this->getCgiPath()); // argv[0]
//    args_vector.push_back(request.getPath()); // argv[1]
//    // Add the query parameters to the argument vector
//    if (request.getParams().size() > 0) {
//        args_vector.push_back("?");
//        for (std::map<std::string, std::string>::const_iterator it = request.getParams().begin();
//             it != request.getParams().end(); ++it) {
//            args_vector.push_back(it->first + "=" + it->second);
//        }
//        args_vector[args_vector.size() - 1] = "&";
//    }
//
    // Add the arguments to the argument pointer array
    char **args = new char *[args_vector.size() + 1];
    for (std::vector<std::string>::size_type i = 0; i < args_vector.size(); ++i) {
        args[i] = new char[args_vector[i].size() + 1];
        std::strcpy(args[i], args_vector[i].c_str());
    }
    args[args_vector.size()] = NULL;
    return args;
}

PhpCgiExecutor::~PhpCgiExecutor() {
}

PhpCgiExecutor::PhpCgiExecutor(const std::string &cgiPath, const std::string &cgiName) : ICgiExecutor(cgiPath,
                                                                                                      cgiName) {

}

PhpCgiExecutor::PhpCgiExecutor() {

}

std::string PhpCgiExecutor::convertToEnvVar(const std::string &header) {
    // Convert from the format "Header-Name" to "HEADER_NAME"
    std::string envVar = "";
    for (std::string::size_type i = 0; i < header.size(); ++i) {
        if (header[i] == '-') {
            envVar += '_';
        } else {
            envVar += std::toupper(header[i]);
        }
    }
    return envVar;
}

