#include "config/ServerConfig.hpp"

ServerConfig::ServerConfig() {

}

ServerConfig::ServerConfig(const ServerConfig &other) {
    *this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        this->_listen = other._listen;
        this->_serverName = other._serverName;
        this->_locations.clear();
        for (std::vector<LocationConfig>::const_iterator it = other._locations.begin();
             it != other._locations.end(); ++it) {
            this->_locations.push_back(LocationConfig(*it));
        }
    }
    return *this;
}

ServerConfig::~ServerConfig() {

}

void ServerConfig::addFunctions(std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)> &functionMap) {
	functionMap["listen"] = &ServerConfig::addListen;
	functionMap["host"] = &ServerConfig::setHost;
	functionMap["server_name"] = &ServerConfig::setServerName;
	//functionMap["index"] = &ServerConfig::addIndex;
	//functionMap["error_page"] = &ServerConfig::;
	functionMap["client_max_body_size"] = &ServerConfig::setClientMaxBodySize;

}

bool ServerConfig::isDelimiter(char c) {
	return (c == '{' || c == '}' || c == ';');
}

std::vector<std::pair<std::string, std::string> > ServerConfig::processFile(const std::string& filename) {
	std::vector<std::pair<std::string, std::string> > result;
	std::ifstream file(filename);


	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		return result;
	}

	std::string line;
	int lineNumber = 1;

	while (std::getline(file, line)) {

		for (size_t i = 0; i < line.size(); ++i) {
			// Handle whitespace characters
			if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\f') {
				// Skip consecutive whitespaces
				continue;
			}
			if (line[i] == '#' || (line[i] == '/' && line[i + 1] == '/'))
				break ;

			// Handle '{', '}', and ';'
			if (line[i] == '{' || line[i] == '}' || line[i] == ';') {
				result.push_back(
						std::make_pair(std::string(1, line[i]), std::to_string(lineNumber) + ":" + std::to_string(i)));
			} else {
				// Handle other characters
				size_t wordStart = i;
				while (i < line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '{' &&
					   line[i] != '}' && line[i] != ';' && line[i] != '\r' && line[i] != '\f') {
					++i;
				}
				std::string word = line.substr(wordStart, i - wordStart);
				result.push_back(std::make_pair(word, std::to_string(lineNumber) + ":" + std::to_string(i)));
				if (line[i] == '{' || line[i] == '}' || line[i] == ';') {
					result.push_back(std::make_pair(std::string(1, line[i]),
													std::to_string(lineNumber) + ":" + std::to_string(i)));
				}
			}
		}

		++lineNumber;
	}

	file.close();
	return result;
}

std::vector<ServerConfig>    ServerConfig::parseConfig(std::string &path) {
	std::ifstream file;
	validate_braces(file);
	std::vector<std::pair<std::string, std::string> > config;
	config = processFile(path);
	std::string pastString;
	std::unordered_multimap<std::string, std::string> myMap;
	std::string position;
	std::vector<ServerConfig> myServers;
	bool startParsing = false;
	for (std::vector<std::pair<std::string, std::string> >::iterator it = config.begin(); it != config.end(); it = config.erase(it)) {
		// Accessing the string
		std::string currentString = it->first;

		// Checking if the string is non-empty
		if (pastString == "server" && currentString == "{") {
			it = config.erase(it);
			startParsing = true;
		}
		if (startParsing)
		{
			for (; it != config.end(); it = config.erase(it)) {
				if (it->first == "location")
				{
					it = config.erase(it);
					getLocation(it, config);
					continue ;
				}
				if (it->first == "}")
					break ;
				pastString = it->first;
				it = config.erase(it);
				for (; it != config.end() && it->first != "}" && it->first != "{" && it->first != ";"; it = config.erase(it))
				{
					myMap.insert(std::make_pair(pastString, it->first));
				}
				if (it->first == "{" || (it->first != ";" && it->first != "}"))
					throw std::runtime_error("Invalid config of server in " + it->second + " --> " + it->first);
				if (it->first == "}")
					break ;
			}
		}
		pastString = currentString;
	}
	std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)> functionMap;
	addFunctions(functionMap);
	std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)>::iterator funcIter;
	for (std::unordered_multimap<std::string, std::string>::iterator it = myMap.begin(); it != myMap.end(); it = myMap.erase(it)) {
		funcIter = functionMap.find(it->first);
		if (funcIter != functionMap.end())
			(this->*funcIter->second)(it->second);
	}
	myServers.push_back(*this);
	return myServers;
}

void ServerConfig::validate_braces(std::ifstream &file) {
	std::stack<std::pair<int, int> > brace_stack;  // (line, column)

	std::string line;
	int line_number = 0;
	while (std::getline(file, line)) {
		line_number++;

		for (size_t i = 0; i < line.size(); i++) {
			if (line[i] == '{') {
				brace_stack.push(std::make_pair(line_number, i + 1));  // i + 1 is the column
			} else if (line[i] == '}') {
				if (brace_stack.empty()) {
					throw std::runtime_error("Error: Unmatched closing brace at line " + std::to_string(line_number) + ", column " + std::to_string(i + 1));
				} else {
					brace_stack.pop();
				}
			}
		}
	}

	while (!brace_stack.empty()) {
		const std::pair<int, int> &position = brace_stack.top();
		throw std::runtime_error("Error: Unmatched opening brace at line " + std::to_string(position.first) + ", column " + std::to_string(position.second));
	}
}

void    ServerConfig::mainSetter(std::unordered_multimap<std::string, std::string> &values){
	for (std::unordered_multimap<std::string, std::string>::iterator it = values.begin(); it != values.end(); it++)
	{
		std::cout << it->first << std::endl;
	}
}

void ServerConfig::getLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config){
	LocationConfig *myLocation = new LocationConfig();
	myLocation->parseLocation(it, config);
	addLocation(*myLocation);
}

void ServerConfig::addListen(int port){
	_listen = port;
}
void ServerConfig::addListen(const std::string& port){
	try {
		_listen = std::stoi(port);
	} catch (std::exception &e) { LOG_ERROR(e.what()); }
}

void ServerConfig::setServerName(const std::string &name) {
	_serverName = name;
}

bool ServerConfig::isListeningOn(int port) {
    return _listen == port;
}

std::string ServerConfig::getServerName() {
    return _serverName;
}

void ServerConfig::removeListen() {
    _listen = -1;
}

void ServerConfig::addLocation(const LocationConfig &location) {
    this->_locations.push_back(location);
}

void ServerConfig::setDefaultLocation(const LocationConfig &location) {
    this->_defaultLocation = location;
}

void ServerConfig::setClientMaxBodySize(size_t size){
	_clientMaxBodySize = size;
}
void ServerConfig::setClientMaxBodySize(const std::string& size){
	try {
		_clientMaxBodySize = std::stol(size);
	} catch (std::exception &e) { std::runtime_error("clientMaxBodySize too large!"); }
}

void ServerConfig::setHost(const std::string& host){
	_host = host;
}