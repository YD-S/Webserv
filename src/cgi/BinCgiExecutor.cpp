//
// Created by kolterdyx on 31/10/23.
//

#include "cgi/BinCgiExecutor.hpp"


bool BinCgiExecutor::executeCgi(HttpRequest *request, std::string *response, std::string scriptPath) {

	// Create a pipe to communicate with the CGI
	// We need to write the request body to the pipe and read the response from the pipe,
	// so we need two file descriptors
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1) {
		LOG_ERROR("Failed to create pipe");
        return false;
	}

	// Build the environment variables
	char **envp_ = buildEnvp(request);

	// Build the arguments
	char **args = buildArgs(request, scriptPath);

	// Fork the process
	pid_t pid = fork();
	if (pid == -1) {
		LOG_ERROR("Failed to fork");
        return false;
	}
	// Child process
	if (pid == 0) {
		// Duplicate the write end of the response_pipe to stdout
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
			LOG_ERROR("Failed to duplicate STDOUT_FILENO pipe");
            exit(1);
		}

		// Duplicate the read end of the pipe_fd to stdin
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1) {
			LOG_ERROR("Failed to duplicate STDIN_FILENO pipe");
            exit(1);
		}

        close(pipe_fd[0]);
        close(pipe_fd[1]);


		// Execute the CGI
		execve(this->getCgiPath().c_str(), args, envp_);
		// If we get here, execve failed
	}
		// Parent process
	else {
		// Write the request body to the pipe
		LOG_DEBUG("Writing to pipe: " << request->getBody());
		if (write(pipe_fd[1], request->getBody().c_str(), request->getBody().size()) == -1) {
			LOG_ERROR("Failed to write to pipe");
            return false;
		}
		// Close the write end of the pipe
        close(pipe_fd[1]);

		bool timeout = false;
#if __APPLE__

		struct kevent ev = {};
		EV_SET(&ev, pipe_fd[0], EVFILT_READ, EV_ADD, 0, 0, NULL);
		struct timespec timeout_s = {TIMEOUT_SEC, 0};
		int kq = kqueue();
		if (kq == -1) {
			LOG_ERROR("Failed to create kqueue");
            return false;
		}
		int num_events = kevent(kq, &ev, 1, &ev, 1, &timeout_s);

#elif __linux__
		// Use epoll_wait to wait for the child process to write to the pipe
		int epoll_fd = epoll_create(1);
		if (epoll_fd == -1) {
			LOG_ERROR("Failed to create epoll");
			return false;
		}
		struct epoll_event event = {};
		event.events = EPOLLIN;
		event.data.fd = pipe_fd[0];
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, pipe_fd[0], &event) == -1) {
			LOG_ERROR("Failed to add pipe to epoll");
			return false;
		}
		struct epoll_event events[1];

		int num_events = epoll_wait(epoll_fd, events, 1, 1000 * TIMEOUT_SEC);
#endif

		if (num_events == -1) {
			LOG_ERROR("Failed to wait for pipe");
            return false;
		}
		if (num_events == 0) {
			LOG_ERROR("Timeout waiting for CGI response");
			timeout = true;
			kill(pid, SIGKILL);
		}
		// Wait for the child process to finish
		int status;
		waitpid(pid, &status, 0);
		if (timeout) {
			return false;
		}
		if (WEXITSTATUS(status) != 0) {
			LOG_ERROR("CGI exited with status " << WEXITSTATUS(status));
			return false;
		}
		// Read the response from the CGI
		char buffer[1024];
		_cgiResult = "";
		ssize_t bytes_read;
		while ((bytes_read = read(pipe_fd[0], buffer, 1024)) > 0) {
			_cgiResult += std::string(buffer, bytes_read);
		}

		close(pipe_fd[0]);
        close(pipe_fd[1]);

		destroyCstrp(envp_);
		destroyCstrp(args);

		*response = postProcessCgiResult();
	}
	return true;
}

char **BinCgiExecutor::buildEnvp(HttpRequest *request) {
	if (!envp) {
		throw std::runtime_error("Environment pointer array is NULL");
	}
	// Build the environment variables
	std::vector<std::string> envp_vector = std::vector<std::string>();
	envp_vector.push_back("GATEWAY_INTERFACE=CGI/1.1");
	envp_vector.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envp_vector.push_back("SERVER_SOFTWARE=Webserv/1.0");
	envp_vector.push_back("SERVER_NAME=" + request->getHeader("Host"));
	envp_vector.push_back("REDIRECT_STATUS=200");
	envp_vector.push_back("REQUEST_METHOD=" + request->getMethod());
	envp_vector.push_back("SCRIPT_NAME=" + request->getPath());
	envp_vector.push_back("PATH_TRANSLATED=" + Webserv::getDirPath(request, _config));
	envp_vector.push_back("BODY=" + request->getBody());
	LOG_DEBUG("BODY: " << request->getBody());

	// Other environment variables
	if (request->getMethod() == "GET") {
		envp_vector.push_back("QUERY_STRING=" + request->getQueryString());
	} else if (request->getMethod() == "POST") {
		envp_vector.push_back("CONTENT_TYPE=" + request->getHeader("Content-Type"));
	}

	// Check required headers like Content-Length and add them if they are missing
	if (request->getHeaders().find("Content-Length") == request->getHeaders().end()) {
		envp_vector.push_back("CONTENT_LENGTH=" + to_string(request->getBody().size()));
	}

	// Add all headers to the environment variables with the HTTP_ prefix
	for (std::map<std::string, std::string>::const_iterator it = request->getHeaders().begin();
		 it != request->getHeaders().end(); ++it) {
		envp_vector.push_back("HTTP_" + convertToEnvVar(it->first) + "=" + it->second);
	}

	// Add the system environment variables to the environment pointer array
	for (int i=0; envp && envp[i]; i++) {
		envp_vector.push_back(std::string(envp[i]));
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

char **BinCgiExecutor::buildArgs(HttpRequest *request, const std::string& scriptPath) {
	// The first argument (omitting the argv[0] executable name) is the file requested
	std::vector<std::string> args_vector = std::vector<std::string>();
	args_vector.push_back(this->getCgiPath()); // argv[0]
	args_vector.push_back(scriptPath); // argv[1]
	// Add the query parameters to the argument vector
	if (request->getParams().size() > 0) {
		args_vector.push_back("?");
		for (std::map<std::string, std::string>::const_iterator it = request->getParams().begin();
			 it != request->getParams().end(); ++it) {
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

BinCgiExecutor::BinCgiExecutor(const std::string &cgiPath, const std::string &cgiName, char **envp, const LocationConfig *config) : ICgiExecutor(
		cgiPath,
		cgiName, envp, config) {

}

BinCgiExecutor::BinCgiExecutor() {
	envp = NULL;
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

