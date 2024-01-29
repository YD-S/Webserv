//
// Created by kolterdyx on 18/01/24.
//

#include "Webserv.hpp"

Webserv::Webserv() {
    LOG_INFO("Webserv created");
}

void Webserv::parseConfig(std::string path) {
    parse = ParseConfig(path);
	parse.parseConfig();
	if (parse.getServers().empty())
			LOG_SYS_ERROR("NO SERVER >:(");
	const std::vector<LocationConfig>& locations = (*(parse.getServers().begin())).getLocations();
	if (locations.empty())
			LOG_SYS_ERROR("NO LOCATIONS >:("); // TODO: even if there are no locations, we should still be able to serve files from the root
}

void Webserv::run() {
    pollManager.socketConfig(parse.getServers());
    pollManager.binder(parse.getServers());
    std::vector<std::pair<HttpResponse *, Client> > responses;
    std::vector<std::pair<HttpRequest *, Client> > requests;

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
		return handleWithLocation(request, &config->getDefaultLocation());
	}
	// Exact match
	for (std::vector<LocationConfig>::const_iterator it = config->getLocations().begin(); it != config->getLocations().end(); ++it) {
		if (request->getPath() == it->getPath()) {
			return handleWithLocation(request, &(*it));
		}
	}
	// Longest prefix match
	LocationConfig *longestPrefixMatch = NULL;
    for (std::vector<LocationConfig>::const_iterator it = config->getLocations().begin(); it != config->getLocations().end(); ++it) {
        if (request->getPath().find(it->getPath()) == 0) {
			if (longestPrefixMatch == NULL || it->getPath().length() > longestPrefixMatch->getPath().length()) {
				longestPrefixMatch = const_cast<LocationConfig *>(&(*it));
			}
        }
    }
	if (longestPrefixMatch != NULL) {
		return handleWithLocation(request, longestPrefixMatch);
	}
	// No match
	return handleWithLocation(request, &config->getDefaultLocation());
}

HttpResponse *Webserv::handleWithLocation(unused const HttpRequest *request, unused const LocationConfig *config) {
    LOG_DEBUG("Handling request with location " << config->getPath());
    HttpResponse *response = new HttpResponse();

	const std::vector<std::string>& methods = config->getMethods();
	if (!methods.empty()) {
		bool methodFound = false;
		for (std::vector<std::string>::const_iterator it = methods.begin(); it != methods.end(); ++it) {
			if (*it == request->getMethod()) {
				methodFound = true;
				break;
			}
		}
		if (!methodFound) {
			LOG_ERROR("Method " << request->getMethod() << " not allowed");
			setErrorResponse(response, HttpStatus::METHOD_NOT_ALLOWED);
			return response;
		}
	}

    return response;
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
    for (std::vector<std::pair<int, const ServerConfig *> >::iterator it = serverSockets.begin(); it != serverSockets.end(); ++it) {
        LOG_DEBUG("Comparing " << it->first << " and " << fd);
        if (it->first == fd) {
            return it->second;
        }
    }
    errno = 0;
    LOG_SYS_ERROR("No server config found for fd " << fd);
    return NULL;
}

void Webserv::setErrorResponse(HttpResponse *response, const int statusCode) {
	response
		->setStatus(statusCode)
		->setHeader("Server", "webserv")
		->setHeader("Content-Type", "text/html")
		->setBody("<html><body><h1>" + to_string(statusCode) + " " + std::string(HttpStatus::getReasonString(statusCode)) + "</h1></body></html>");
}

