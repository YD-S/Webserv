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
			LOG_ERROR("NO SERVER >:(");
	const std::vector<LocationConfig>& locations = (*(parse.getServers().begin())).getLocations();
	if (locations.empty())
			LOG_ERROR("NO LOCATIONS >:(");
	parse.printAll();
}

void Webserv::run() {
    pollManager.socketConfig(parse.getServers());
    pollManager.binder(parse.getServers());
    std::vector<std::pair<const HttpResponse *, const Client *> > responses;
    std::vector<std::pair<const HttpRequest *, const Client *> > requests;

    while (1) {
        pollManager.poller();
        requests = pollManager.getRequests();
        if (requests.empty()) {
            continue;
        }
        LOG_DEBUG("Requests: " << requests.size());
        responses = pollManager.getResponses();
        for (unsigned long i = 0; i < requests.size(); i++) {
            std::cout << requests[i].first->toPrintableString() << std::endl;
//            const HttpResponse response = handleRequest(requests[i].first, getServerConfigByFd(requests[i].first->getFd()));
            HttpResponse response = HttpResponse();
            response.setStatus(HttpStatus::OK)
                    .setHeader("Server", "webserv")
                    .setHeader("Content-Type", "text/html")
                    .setBody("<html><body><h1>Hello, world!</h1><p>" + to_string(rand()) + "</p></body></html>");
            responses.push_back(std::make_pair(&response, requests[i].second));
            pollManager.setRequestHandled(requests[i].first);
        }
        LOG_DEBUG("Responses: " << responses.size());
        pollManager.setResponses(responses);
    }
}

const HttpResponse Webserv::handleRequest(unused const HttpRequest *request, unused const ServerConfig *config) {
    if (!config->getLocations().size())
        return handleWithLocation(request, &config->getDefaultLocation());
    for (std::vector<LocationConfig>::const_iterator it = config->getLocations().begin(); it != config->getLocations().end(); ++it) {
        if (request->getPath().find(it->getPath()) == 0) {
            return handleWithLocation(request, &(*it));
        }
    }
    errno = ENOENT;
    LOG_ERROR("No location found for path " << request->getPath());
    return HttpResponse().setStatus(HttpStatus::IM_A_TEAPOT).setHeader("Server", "webserv").setHeader("Content-Type", "text/html").setBody("<html><body><h1>I'm a teapot</h1></body></html>");
}

const HttpResponse Webserv::handleWithLocation(unused const HttpRequest *request, unused const LocationConfig *config) {
    LOG_INFO("Handling request with location " << config->getPath());
    HttpResponse response = HttpResponse();
    response
            .setStatus(HttpStatus::OK)
            .setHeader("Server", "webserv")
            .setHeader("Content-Type", "text/html")
            .setBody("<html><body><h1>Hello, world!</h1><p>" + to_string(rand()) + "</p></body></html>");
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
    LOG_ERROR("No server config found for fd " << fd);
    return NULL;
}

