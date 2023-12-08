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
    std::ifstream _file;
	fileValidator.validate_config_file(_file);
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
						std::make_pair(std::string(1, line[i]), to_string(lineNumber) + ":" + to_string(i)));
			} else {
				// Handle other characters
				size_t wordStart = i;
				while (i < line.size() && line[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '{' &&
					   line[i] != '}' && line[i] != ';' && line[i] != '\r' && line[i] != '\f') {
					++i;
				}
				std::string word = line.substr(wordStart, i - wordStart);
				result.push_back(std::make_pair(word, to_string(lineNumber) + ":" + to_string(i)));
				if (line[i] == '{' || line[i] == '}' || line[i] == ';') {
					result.push_back(std::make_pair(std::string(1, line[i]),
													to_string(lineNumber) + ":" + to_string(i)));
				}
			}
		}

		++lineNumber;
	}
	for (std::vector<std::pair<std::string, std::string> >::iterator it = result.begin(); it != result.end(); ++it) {
        std::cout << "Key: " << it->first << "\t\tValue: " << it->second << std::endl;
    }

	_file.close();
	return result;
}

void    ParseConfig::parseConfig() {
	std::vector<std::pair<std::string, std::string> > config;
	config = processFile();
	std::string pastString;
	std::string currentString;
	std::string position;
	std::vector<std::pair<std::string, std::vector <std::string> > > variables;
	std::string lastVar;
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
				std::vector <std::string> variablesVar;
                if (it->first == "location")
                {
                    it = config.erase(it);
                    parseLocation(it, config);
                    continue ;
                }
                if (it->first == "}")
                    break ;
                if (pastString == "}" && lastVar != ";" && lastVar != "{" && !lastVar.empty())
					throw std::runtime_error("Invalid config of server in " + it->second + " --> " + it->first);
				pastString = it->first;
                it = config.erase(it);
                for (; it != config.end() && it->first != "}" && it->first != "{" && it->first != ";"; it = config.erase(it))
                {
                    variablesVar.push_back(it->first);
                }
				variables.push_back(std::make_pair(pastString, variablesVar));
                if (it->first == "{" || (it->first != ";" && it->first != "}"))
                    throw std::runtime_error("Invalid config of server in " + it->second + " --> " + it->first);
                if (it->first == "}")
                    break ;
				lastVar = it->first;
            }
			//mainSetter(variables);
        }
        else {
            LOG_ERROR("AYO WTF");
			exit(-1);
		}
		//printTempVariables(variables);
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
					throw std::runtime_error("Error: Unmatched closing brace at line " + to_string(line_number) + ", column " + to_string(i + 1));
				} else {
					brace_stack.pop();
				}
			}
		}
	}

	while (!brace_stack.empty()) {
		const std::pair<int, int> &position = brace_stack.top();
		throw std::runtime_error("Error: Unmatched opening brace at line " + to_string(position.first) + ", column " + to_string(position.second));
	}
}

LocationConfig ParseConfig::parseLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config) {
	std::vector<std::pair<std::string, std::vector <std::string> > > variables;
	
	LocationConfig location;

	if (it->first.at(0) != '/')
		throw std::runtime_error("Invalid path of location in " + it->second + " --> " + it->first);
	location.setPath(it->first);
	it = config.erase(it);
	if (it->first != "{")
		throw std::runtime_error("Invalid config of location in " + it->second + " --> " + it->first + " | Expected \"{\"");
	it = config.erase(it);
	std::string pastString;
	for (; it->first != "}"; it = config.erase(it)) {
		std::vector <std::string> variablesVar;
		pastString = it->first;
		it = config.erase(it);
		for (; it->first != "}" && it->first != "{" && it->first != ";"; it = config.erase(it))
		{
			variablesVar.push_back(it->first);
		}
		variables.push_back(std::make_pair(pastString, variablesVar));
		if (it->first == "{" || (it->first != ";" && it->first != "}"))
			throw std::runtime_error("Invalid config of location in " + it->second + " --> " + it->first);
	}
	return location;
}

void    ParseConfig::mainSetter(std::vector<std::pair<std::string, std::vector <std::string> > >& variables){
	LocationConfig defaultLocation;
	
	std::vector<std::pair<std::string, std::vector <std::string> > >::iterator it = variables.begin();
	while (it != variables.end()){
		
		if (it->first == "listen")
		{
			
		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		else if (it->first == "listen")
		{

		}
		
		for (std::vector <std::string>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
			
		}
		std::cout << "IM GEI "<< std::endl;
	}
}


std::vector<ServerConfig>& ParseConfig::getServers(){
    return _servers;
}

void    ParseConfig::printTempVariables(std::vector<std::pair<std::string, std::vector <std::string> > > variables){
	for (std::vector<std::pair<std::string, std::vector <std::string> > >::iterator it = variables.begin(); it != variables.end(); ++it){
		std::cout << it->first << ": " << std::endl;
		for (std::vector <std::string>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
			std::cout << *iter << ", ";
		}
		std::cout << "IM NOT GEI "<< std::endl;
	}
}