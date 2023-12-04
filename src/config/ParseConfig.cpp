#include "../includes/config/ParseConfig.hpp"

ParseConfig::ParseConfig() {

}

ParseConfig::ParseConfig(const std::string& path) : _path(path) {
    
}

ParseConfig::ParseConfig(const ParseConfig &other) {
    *this = other;
}

ParseConfig &ParseConfig::operator=(const ParseConfig &other) {
    if (this != &other) {
        this->_servers.clear();

            // Copy data from other._servers using const iterator
            for (std::vector<ServerConfig>::const_iterator it = other._servers.begin(); it != other._servers.end(); ++it) {
                this->_servers.push_back(*it);
            }
    }
    return *this;
}

ParseConfig::~ParseConfig() {

}



bool ParseConfig::isDelimiter(char c) {
	return (c == '{' || c == '}' || c == ';');
}

std::vector<std::pair<std::string, std::string> > ParseConfig::processFile() {
	std::vector<std::pair<std::string, std::string> > result;
	fileValidate fileValidator(_path);
    std::ifstream _file = fileValidator.validate_config_file();
    validate_braces(_file);
    _file.clear();
    _file.seekg(0);

	std::string line;
	int lineNumber = 1;

	while (std::getline(_file, line)) {

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

	_file.close();
	return result;
}

void ParseConfig::addFunctionsServer(std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)> &functionMap) {
	functionMap["listen"] = &ServerConfig::addListen;
	functionMap["host"] = &ServerConfig::setHost;
	functionMap["server_name"] = &ServerConfig::setServerName;
	//functionMap["index"] = &ServerConfig::addIndex;
	functionMap["error_pages"] = &ServerConfig::setErrorPages;
	functionMap["client_max_body_size"] = &ServerConfig::setClientMaxBodySize;
}

void    ParseConfig::parseConfig() {
	std::vector<std::pair<std::string, std::string> > config;
	config = processFile();
	std::string pastString;
	std::string currentString;
    std::unordered_multimap<std::string, std::string> myMap;
	std::string position;
    for (std::vector<std::pair<std::string, std::string> >::iterator it = config.begin(); it != config.end(); it = config.erase(it)){
        ServerConfig server;
        // Accessing the string
        pastString.clear();
        pastString = it->first;
        it = config.erase(it);
        currentString.clear();
        currentString = it->first;
        // Checking if the string is non-empty
        it = config.erase(it);
        if (pastString == "server" && currentString == "{") {
            for (; it->first != "}"; it = config.erase(it)) {
                if (it->first == "location")
                {
                    it = config.erase(it);
                    server.setLocation(it, config);
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
        else
            LOG_ERROR("AYO WTF");
        std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)> functionMap;
        addFunctionsServer(functionMap);
        std::unordered_map<std::string, void (ServerConfig::*)(const std::string&)>::iterator funcIter;
        for (std::unordered_multimap<std::string, std::string>::iterator iter = myMap.begin(); iter != myMap.end(); iter = myMap.erase(iter)) {
            funcIter = functionMap.find(iter->first);
            if (funcIter != functionMap.end())
                (server.*funcIter->second)(iter->second);
        }
        server.setDefaultLocation(server.getLocations()[0]);
        _servers.push_back(server);    
    }
}

void ParseConfig::validate_braces(std::ifstream &file) {
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

void    ParseConfig::mainSetter(std::unordered_multimap<std::string, std::string> &values){
	for (std::unordered_multimap<std::string, std::string>::iterator it = values.begin(); it != values.end(); it++)
	{
		std::cout << it->first << std::endl;
	}
}


std::vector<ServerConfig>& ParseConfig::getServers(){
    return _servers;
}