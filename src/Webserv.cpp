//
// Created by kolterdyx on 18/01/24.
//

#include "Webserv.hpp"
#include "MimeTypes.hpp"
#include "cgi/BinCgiExecutor.hpp"

Webserv::Webserv() {
	LOG_INFO("Webserv created");
}

void Webserv::parseConfig(const std::string &path) {
	parse = ParseConfig(path);
	parse.parseConfig();
	if (parse.getServers().empty())
		LOG_SYS_ERROR("NO SERVER :(\t");
    parse.printAll();
}

void Webserv::run(char **envp) {
	pollManager.socketConfig(parse.getServers());
	pollManager.binder(parse.getServers());
	std::vector<std::pair<HttpResponse *, Client> > responses;
	std::vector<std::pair<HttpRequest *, Client> > requests;

	// for each config, find all the cgi configs and create a cgi executor for each
	for (std::vector<ServerConfig>::const_iterator it = parse.getServers().begin();
		 it != parse.getServers().end(); ++it) {
		for (std::vector<LocationConfig>::const_iterator it2 = it->getLocations().begin();
			 it2 != it->getLocations().end(); ++it2) {
			if (it2->isCgiEnabled()) {
				std::vector<std::pair<std::string, std::string> > cgis = it2->getCgi();
				for (std::vector<std::pair<std::string, std::string> >::const_iterator it3 = cgis.begin();
					 it3 != cgis.end(); ++it3) {
					std::string extension = it3->first;
					std::string path = it3->second;
					ICgiExecutor *cgiExecutor = new BinCgiExecutor(path, extension, envp, &(*it2));
					cgiExecutors[extension] = cgiExecutor;
				}
			}
		}
	}

	while (1) {
		pollManager.poller();
		requests = pollManager.getRequests();
		if (requests.empty()) {
			continue;
		}
		LOG_DEBUG("Requests: " << requests.size());
		responses = pollManager.getResponses();
		for (unsigned long i = 0; i < requests.size(); i++) {
			HttpResponse *response = handleRequest(requests[i].first, getServerConfigByFd(requests[i].first->getFd()));
			responses.push_back(std::make_pair(response, requests[i].second));
			pollManager.setRequestHandled(requests[i].first);
		}
		LOG_DEBUG("Responses: " << responses.size());
		pollManager.setResponses(responses);
	}
}

HttpResponse *Webserv::handleRequest(const HttpRequest *request, unused const ServerConfig *config) {
    if (config->getLocations().empty()) {
		return handleWithLocation(request, config->getDefaultLocation());
	}
	// Exact match
	for (std::vector<LocationConfig>::const_iterator it = config->getLocations().begin();
		 it != config->getLocations().end(); ++it) {
		if (request->getPath() == it->getPath()) {
			LocationConfig *location = const_cast<LocationConfig *>(&(*it));
			location->setDefaultConfig(const_cast<LocationConfig *>(config->getDefaultLocation()));
			return handleWithLocation(request, location);
		}
	}
	// Longest prefix match
	LocationConfig *longestPrefixMatch = NULL;
	for (std::vector<LocationConfig>::const_iterator it = config->getLocations().begin();
		 it != config->getLocations().end(); ++it) {
		if (request->getPath().find(it->getPath()) == 0) {
			if (longestPrefixMatch == NULL || it->getPath().length() > longestPrefixMatch->getPath().length()) {
				longestPrefixMatch = const_cast<LocationConfig *>(&(*it));
			}
		}
	}
	if (longestPrefixMatch != NULL) {
		longestPrefixMatch->setDefaultConfig(const_cast<LocationConfig *>(config->getDefaultLocation()));
		return handleWithLocation(request, longestPrefixMatch);
	}
	// No match
	return handleWithLocation(request, config->getDefaultLocation());
}

HttpResponse *Webserv::handleWithLocation(unused const HttpRequest *request, unused const LocationConfig *config) {
	try {
		LOG_DEBUG("Handling request with location " << config->getPath());
		HttpResponse *response = new HttpResponse(); // leaks memory
		response->setHeader("Server", "webserv");

        if (request->isInvalid(const_cast<LocationConfig *>(config))) {
            LOG_ERROR("Invalid request");
            setErrorResponse(response, request->isInvalid(const_cast<LocationConfig *>(config)), const_cast<LocationConfig *>(config));
            return response;
        }

		if (!checkRequestMethod(request, config, response))
			return response;

		if (!config->getRedirect().empty()) {
			LOG_DEBUG("Redirect enabled");
			return response->setStatus(HttpStatus::MOVED_PERMANENTLY)->setHeader("Location", config->getRedirect());
		}
		if ((request->getMethod() == "GET" || request->getMethod() == "POST") &&
			config->isCgiEnabled() &&
			!is_dir(getDirPath(request, config))
				) {
			LOG_DEBUG("Using CGI");
			return handleWithCGI(request, config, response);
		}

		if (request->getMethod() == "GET" && config->isDirectoryListingEnabled() &&
			request->getPath().at(request->getPath().length() - 1) == '/') {
			LOG_DEBUG("Directory listing enabled");
			return generateDirectoryListing(request, config);
		}

		if (request->getMethod() == "GET") {
			LOG_DEBUG("GET request");
			if (is_dir(getDirPath(request, config)) && !config->getIndexes().empty() && config->isAutoIndexEnabled()) {
				LOG_DEBUG("Autoindex listing enabled");
				return getIndex(const_cast<HttpRequest *>(request), config, response);
			}
			return getFile(request, config, response);
		}

        if (request->getMethod() == "POST") {
            LOG_DEBUG("POST request");
            std::ofstream file(getDirPath(request, config).append(config->getUploadPath()).append("/").append(request->getHeader("file-name")).c_str(), std::ios::binary);
            LOG_DEBUG("Upload path: " << getDirPath(request, config).append(config->getUploadPath()).append("/").append(request->getHeader("file-name")));
            if (file.is_open())
            {
                response->setHeader("Connection", "keep-alive");

                file.write(request->getBody().c_str(), request->getBody().size());
                file.close();
                response->setStatus(HttpStatus::OK);
            }
            else
            {
                LOG_SYS_ERROR("Error opening file while POST " << getDirPath(request, config).append(config->getUploadPath()).append("/").append(request->getHeader("File-Name")));
                response->setStatus(409);
            }
            return response;
        }

		if (request->getMethod() == "DELETE") {
			LOG_DEBUG("DELETE request");
			std::string path = getDirPath(request, config);
			if (remove(path.c_str()) != 0) {
				LOG_SYS_ERROR("Error deleting file " << path);
				setErrorResponse(response, HttpStatus::INTERNAL_SERVER_ERROR, const_cast<LocationConfig *>(config));
				return response;
			}
			return response->setStatus(HttpStatus::NO_CONTENT);
		}
		LOG_WARNING("Using default response");
        setDefaultResponse(response, const_cast<LocationConfig *>(config));
		return response;
	} catch (std::exception &e) {
		LOG_SYS_ERROR("Error handling request: " << e.what());
		HttpResponse *response = new HttpResponse();
		setErrorResponse(response, HttpStatus::INTERNAL_SERVER_ERROR, const_cast<LocationConfig *>(config));
		return response;
	}
}

HttpResponse *Webserv::handleWithCGI(const HttpRequest *request, const LocationConfig *config, HttpResponse *response) {
	std::string extension;
	size_t pos = request->getPath().find_last_of('.');
	if (pos != std::string::npos) {
		extension = request->getPath().substr(pos);
	}
	ICgiExecutor *cgiExecutor;
	try {
		cgiExecutor = cgiExecutors.at(extension);
	} catch (std::out_of_range &e) {
		LOG_DEBUG("No CGI executor found for extension " << extension);
		return getFile(request, config, response);
	}
	std::string responseString;
	LOG_DEBUG("Executing CGI");
	delete response;
	response = new HttpResponse();
	if (!cgiExecutor->executeCgi(const_cast<HttpRequest *>(request), &responseString, getDirPath(request, config))){
		setErrorResponse(response, HttpStatus::INTERNAL_SERVER_ERROR, const_cast<LocationConfig *>(config));
		return response;
	}
	response->fromString(responseString);
	response->setHeader("Server", "webserv");
	return response;
}

bool Webserv::checkRequestMethod(const HttpRequest *request, const LocationConfig *config, HttpResponse *response) {
	const std::vector<std::string> &methods = config->getMethods();
	if (!methods.empty()) {
		if (!config->hasMethod(request->getMethod())) {
			LOG_ERROR("Method " << request->getMethod() << " not allowed");
			setErrorResponse(response, HttpStatus::METHOD_NOT_ALLOWED, const_cast<LocationConfig *>(config));
			return false;
		}
	}
	return true;
}

Webserv::Webserv(const Webserv &other) {
	*this = other;
}

Webserv &Webserv::operator=(const Webserv &other) {
	if (this != &other) {
		this->parse = other.parse;
		this->pollManager = other.pollManager;
	}
	return *this;
}

Webserv::~Webserv() {
	LOG_DEBUG("Webserv destroyed");
}

const ServerConfig *Webserv::getServerConfigByFd(int fd) {
	for (std::vector<std::pair<int, const ServerConfig *> >::iterator it = serverSockets.begin();
		 it != serverSockets.end(); ++it) {
		LOG_DEBUG("Comparing " << it->first << " and " << fd);
		if (it->first == fd) {
			return it->second;
		}
	}
	errno = 0;
	LOG_SYS_ERROR("No server config found for fd " << fd);
	return NULL;
}

void Webserv::setErrorResponse(HttpResponse *response, const int statusCode, LocationConfig *config) {
	std::string body =
			"<html><body><h1>" + to_string(statusCode) + " " + std::string(HttpStatus::getReasonString(statusCode)) +
			"</h1></body></html>";
	if (!config->getErrorPage(statusCode).empty()) {
		LOG_DEBUG("Error page found for status code " << statusCode);

		std::string errorPagePath = config->getRoot();
		if (errorPagePath.at(errorPagePath.length() - 1) != '/')
			errorPagePath += "/";
		std::string errorPage = config->getErrorPage(statusCode);
		if (errorPage.at(0) == '/')
			errorPage = errorPage.substr(1);
		errorPagePath += errorPage;
		std::ifstream errorPageFile(errorPagePath.c_str());
		if (errorPageFile) {
			body.clear();
			while (errorPageFile.good()) {
				std::string line;
				std::getline(errorPageFile, line);
				body += line;
			}
			errorPageFile.close();
		} else {
			LOG_SYS_ERROR("Error opening error page file " << errorPagePath);
			LOG_WARNING("Using default error page");
		}
	}
	response
			->setStatus(statusCode)
			->setHeader("Content-Type", "text/html")
			->setBody(body);
}

void Webserv::setDefaultResponse(HttpResponse *response, LocationConfig *config) {

	std::string locationString = config->toString();

	std::string body = "<html><body><h1>Default response</h1>";

	body += "<h2>Random number (refresh test)</h2>";
	body += "<pre>" + to_string(rand()) + "</pre>";

	body += "<h2>Location</h2>";
	body += "<pre>" + locationString + "</pre>";

	body += "</body></html>";

	response
			->setStatus(HttpStatus::OK)
			->setHeader("Content-Type", "text/html")
			->setBody(body);
}

HttpResponse *Webserv::generateDirectoryListing(const HttpRequest *request, const LocationConfig *config) {
	HttpResponse *response = new HttpResponse();
	response
			->setStatus(HttpStatus::OK)
			->setHeader("Content-Type", "text/html");

	std::string body =
			"<html><head><style>th { font-size: larger; font-weight: bold; text-align: left; }</style></head><body><h1>Index of " +
			request->getPath() + "</h1>";
	std::string path = getDirPath(request, config);
	DIR *dir = opendir(path.c_str()); // Leaks memory
	if (dir == NULL) {
		LOG_SYS_ERROR("Error opening directory " << path);
		setErrorResponse(response, HttpStatus::INTERNAL_SERVER_ERROR, const_cast<LocationConfig *>(config));
		return response;
	}

	body += "<table><thead><tr><th>Permissions</th><th>Links</th><th>Size</th><th>Name</th></tr></thead><tbody>";
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		if (to_string(entry->d_name) == ".") // Skip current directory
			continue;
		// Add file info
		struct stat fileStat;
		std::string fullpath = path + entry->d_name;
		stat(fullpath.c_str(), &fileStat);
		body += "<tr><td>";
		body += is_dir(fullpath) ? "d" : "-";
		body += (fileStat.st_mode & S_IRUSR) ? "r" : "-";
		body += (fileStat.st_mode & S_IWUSR) ? "w" : "-";
		body += (fileStat.st_mode & S_IXUSR) ? "x" : "-";
		body += (fileStat.st_mode & S_IRGRP) ? "r" : "-";
		body += (fileStat.st_mode & S_IWGRP) ? "w" : "-";
		body += (fileStat.st_mode & S_IXGRP) ? "x" : "-";
		body += (fileStat.st_mode & S_IROTH) ? "r" : "-";
		body += (fileStat.st_mode & S_IWOTH) ? "w" : "-";
		body += (fileStat.st_mode & S_IXOTH) ? "x" : "-";
		body += "</td><td>";
		body += to_string(fileStat.st_nlink);
		body += "</td><td>";
		body += to_string(fileStat.st_size);
		body += "</td><td>";
		if (is_dir(fullpath))
			body += "<a href='" + request->getPath() + entry->d_name + "/'>" + entry->d_name + "/</a><br>";
		else
			body += "<a href='" + request->getPath() + entry->d_name + "'>" + entry->d_name + "</a><br>";
		body += "</td></tr>";
	}
	body += "</tbody></table></body></html>";

	response->setBody(body);

	return response;
}

std::string Webserv::getDirPath(const HttpRequest *request, const LocationConfig *config) {
	std::string rootPath = config->getRoot();
	if (rootPath.at(rootPath.length() - 1) != '/')
		rootPath += "/";
	// subtract prefix from path
	std::string path = rootPath + request->getPath().substr(config->getPath().length());
	return path;
}

HttpResponse *Webserv::getFile(const HttpRequest *request, const LocationConfig *config, HttpResponse *response) {
	std::string path = getDirPath(request, config);
	std::string extension;
	size_t pos = path.find_last_of('.');
	if (pos != std::string::npos) {
		extension = path.substr(pos + 1);
	}
	LOG_DEBUG("Path: " << path);
	int exists = fileExists(path);
	if (exists == 0) {
		LOG_ERROR("File " << path << " not found");
		setErrorResponse(response, HttpStatus::NOT_FOUND, const_cast<LocationConfig *>(config));
		return response;
	} else if (exists < 0) {
		LOG_SYS_ERROR("Error opening file " << path);
		setErrorResponse(response, HttpStatus::INTERNAL_SERVER_ERROR, const_cast<LocationConfig *>(config));
		return response;
	}
	response->setHeader("Content-Type", MimeTypes::getType(extension));
	response->setHeader("Connection", "keep-alive");
	response->setStatus(HttpStatus::OK);
	if (isBinaryFile(path)) {
		LOG_DEBUG("Binary file");
		std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
		if (file != NULL) {
			if (file.good()) {
				std::string binaryData;
				while (file.good()) {
					char c;
					file.get(c);
					binaryData += c;
				}
				response->setBody(binaryData);
			}
			file.close();
		}
		return response;
	}
	LOG_DEBUG("Text file");
	std::ifstream file(path.c_str(), std::ios::in);
	std::string body;
	if (file != NULL) {
		if (file.good()) {
			std::getline(file, body, '\0');
		}
		file.close();
	}
	response->setBody(body);

	return response;
}

HttpResponse *Webserv::getIndex(HttpRequest *request, const LocationConfig *config, HttpResponse *response) {
	// Try each index in order
	for (std::vector<std::string>::const_iterator it = config->getIndexes().begin();
		 it != config->getIndexes().end(); ++it) {
		std::string path = getDirPath(request, config);
		if (path.at(path.length() - 1) != '/')
			path += "/";
		path += *it;
		LOG_DEBUG("Trying index " << path);
		int exists = fileExists(path);
		if (exists == 0) {
			LOG_DEBUG("Index " << path << " not found");
			continue;
		} else if (exists < 0) {
			LOG_SYS_ERROR("Error opening index " << path);
			setErrorResponse(response, HttpStatus::INTERNAL_SERVER_ERROR, const_cast<LocationConfig *>(config));
			return response;
		}
		LOG_DEBUG("Index " << path << " found");
		std::string requestPath = request->getPath();
		if (requestPath.at(requestPath.length() - 1) != '/')
			requestPath += "/";
		request->setPath(requestPath + *it);
		if (config->isCgiEnabled())
			return handleWithCGI(request, config, response);
		return getFile(request, config, response);
	}
	LOG_ERROR("No index found");
	setErrorResponse(response, HttpStatus::NOT_FOUND, const_cast<LocationConfig *>(config));
	return response;
}
