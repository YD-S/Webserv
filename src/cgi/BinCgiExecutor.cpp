//
// Created by kolterdyx on 31/10/23.
//

#include <stdexcept>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include "cgi/BinCgiExecutor.hpp"
#include "macros.h"
#include "utils.hpp"
#include "../../includes/cgi/BinCgiExecutor.hpp"


void BinCgiExecutor::executeCgi(HttpRequest &request, std::string &response) {

    // Create a pipe to communicate with the CGI
    // We need to write the request body to the pipe and read the response from the pipe,
    // so we need two file descriptors
    int request_pipe[2];
    int response_pipe[2];
    if (pipe(request_pipe) == -1 || pipe(response_pipe) == -1) {
        throw std::runtime_error("Failed to create pipe");
    }

    // Build the environment variables
    char **envp_ = buildEnvp(request);

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
        execve(this->getCgiPath().c_str(), args, envp_);

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

        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);

        // Read the response from the CGI
        char buffer[1024];
        _cgiResult = "";
        ssize_t bytes_read;
        while ((bytes_read = read(response_pipe[0], buffer, 1024)) > 0) {
            _cgiResult += std::string(buffer, bytes_read);
        }

        // Close the read end of the pipe
        close(response_pipe[0]);

        destroyCstrp(envp_);
        destroyCstrp(args);

        response = postProcessCgiResult();
    }


}

char **BinCgiExecutor::buildEnvp(HttpRequest &request) {
    // Build the environment variables
    std::vector<std::string> envp_vector = std::vector<std::string>();
    envp_vector.push_back("GATEWAY_INTERFACE=CGI/1.1");
    envp_vector.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envp_vector.push_back("REDIRECT_STATUS=200");
    envp_vector.push_back("REQUEST_METHOD=" + request.getMethod());
    envp_vector.push_back("SCRIPT_FILENAME=" + request.getPath());
    envp_vector.push_back("BODY=" + request.getBody());

    // Other environment variables
    if (request.getMethod() == "GET") {
        envp_vector.push_back("QUERY_STRING=" + request.getQueryString());
    } else if (request.getMethod() == "POST") {
        envp_vector.push_back("CONTENT_TYPE=" + request.getHeaders().at("Content-Type"));
    }

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
    for (char **env = envp; *env != NULL; ++env) {
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

void BinCgiExecutor::destroyCstrp(char **cstrp) {
    for (char **str = cstrp; *str != NULL; ++str) {
        delete[] *str;
    }
    delete[] cstrp;
}

char **BinCgiExecutor::buildArgs(unused HttpRequest &request) {
    // The first argument (omitting the argv[0] executable name) is the file requested
    std::vector<std::string> args_vector = std::vector<std::string>();
    args_vector.push_back(this->getCgiPath()); // argv[0]
    args_vector.push_back(request.getPath()); // argv[1]
    // Add the query parameters to the argument vector
    if (request.getParams().size() > 0) {
        args_vector.push_back("?");
        for (std::map<std::string, std::string>::const_iterator it = request.getParams().begin();
             it != request.getParams().end(); ++it) {
            args_vector.push_back(it->first + "=" + it->second);
        }
        args_vector[args_vector.size() - 1] = "&";
    }

    // Add the arguments to the argument pointer array
    char **args = new char *[args_vector.size() + 1];
    for (std::vector<std::string>::size_type i = 0; i < args_vector.size(); ++i) {
        args[i] = new char[args_vector[i].size() + 1];
        std::strcpy(args[i], args_vector[i].c_str());
    }
    args[args_vector.size()] = NULL;
    return args;
}

BinCgiExecutor::~BinCgiExecutor() {
}

BinCgiExecutor::BinCgiExecutor(const std::string &cgiPath, const std::string &cgiName, char **envp) : ICgiExecutor(cgiPath,
                                                                                                      cgiName, envp) {

}

BinCgiExecutor::BinCgiExecutor() {

}

std::string BinCgiExecutor::convertToEnvVar(const std::string &header) {
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

std::string BinCgiExecutor::postProcessCgiResult() {
    std::string result = "";
    std::string status_line = "";
    std::string::size_type status_line_start = _cgiResult.find("Status: ");
    if (status_line_start != std::string::npos) {
        status_line_start += 8;
        // Find the end of the line
        std::string::size_type status_line_end = _cgiResult.find("\n", status_line_start);
        if (status_line_end != std::string::npos) {
            status_line = _cgiResult.substr(status_line_start, status_line_end - status_line_start);
        }
    } else {
        status_line = "200 OK";
    }
    result += "HTTP/1.1 " + status_line + "\n";
    // Add everything except the status line to the result
    if (_cgiResult.find("Status: ") != std::string::npos) {
        result += _cgiResult.substr(0, _cgiResult.find("Status: "));
        result += _cgiResult.substr(status_line_start + status_line.size() + 1);
    } else {
        result += _cgiResult;
    }
    return result;
}

