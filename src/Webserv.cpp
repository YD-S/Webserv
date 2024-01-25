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
            HttpResponse *response = new HttpResponse();
            response->setVersion(requests[i].first->getVersion());
            response->setStatus(HttpStatus::OK);
            response->addHeader("Server", "webserv");
            response->addHeader("Content-Type", "text/html");
            response->setBody("<html><body><h1>Hello, world!</h1><p>" + to_string(rand()) + "</p></body></html>");

            responses.push_back(std::make_pair(response, requests[i].second));
            pollManager.setRequestHandled(requests[i].first);
        }
        LOG_DEBUG("Responses: " << responses.size());
        pollManager.setResponses(responses);
    }
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
