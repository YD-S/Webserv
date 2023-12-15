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
            for (std::vector<ServerConfig>::const_iterator it = other._servers.begin(); it != other._servers.end(); ++it) {
                this->_servers.push_back(*it);
            }
		this->_path = other._path;
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
        pastString.clear();
        pastString = it->first;
        it = config.erase(it);
        currentString.clear();
        currentString = it->first;
        it = config.erase(it);
        if (pastString == "server" && currentString == "{") {
            for (; it->first != "}"; it = config.erase(it)) {
				std::vector <std::string> variablesVar;
                if (it->first == "location")
                {
                    it = config.erase(it);
                    server.addLocation(parseLocation(it, config));
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
		mainSetter(variables);
		const std::vector<LocationConfig>& locations = server.getLocations();
        for (std::vector<LocationConfig>::const_iterator locationIt = locations.begin(); locationIt != locations.end(); ++locationIt) {
            const LocationConfig& location = *locationIt;

            std::cout << "Location:" << std::endl;
            std::cout << "  Path: " << location.getPath() << std::endl;
            std::cout << "  Root: " << location.getRoot() << std::endl;

            // Print Location Details (add more details as needed)
            std::cout << "  Indexes: ";
            const std::vector<std::string>& indexes = location.getIndexes();
            for (std::vector<std::string>::const_iterator indexIt = indexes.begin(); indexIt != indexes.end(); ++indexIt) {
                const std::string& index = *indexIt;
                std::cout << index << " ";
            }
            std::cout << std::endl;

            std::cout << "  Methods: ";
            const std::vector<std::string>& methods = location.getMethods();
            for (std::vector<std::string>::const_iterator methodIt = methods.begin(); methodIt != methods.end(); ++methodIt) {
                const std::string& method = *methodIt;
                std::cout << method << " ";
            }
            std::cout << std::endl;

            // Add more details if needed
        }
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

std::string ParseConfig::extractAndRemoveFirst(std::vector<std::string>& values) {
    if (!values.empty()) {
        // Retrieve the first string from the vector
        std::string extractedString = values[0];

        // Remove the first element from the vector
        values.erase(values.begin());

        // Return the extracted string
        return extractedString;
    } else {
        // If the vector is empty, return an empty string or handle it as needed
        return "";
    }
}

void ParseConfig::separateHostPort(ServerConfig &server, const std::string& input) {
    size_t colonPos = input.find(':');

    if (colonPos != std::string::npos) {
		if (input.substr(0, colonPos).empty()){
			LOG_ERROR("host not specified!");
			exit(1);
		}
		if (input.substr(colonPos + 1).empty()){
			LOG_ERROR("port not specified!");
			exit(1);
		}
        server.setHostPort(input.substr(0, colonPos), stoi(input.substr(colonPos + 1)));
    } else {
        LOG_ERROR("host syntax error!");
		exit(1);
    }
}

void    ParseConfig::mainSetter(std::vector<std::pair<std::string, std::vector <std::string> > >& variables){
	ServerConfig server;
	LocationConfig defaultLocation;
	
	std::vector<std::pair<std::string, std::vector <std::string> > >::iterator temp;
	std::vector<std::pair<std::string, std::vector <std::string> > >::iterator it = variables.begin();
	for (; it != variables.end(); it = variables.erase(it)){
		
		if (it->first == "server_name")
		{
			server.setServerName(extractAndRemoveFirst(it->second));
			if (!it->second.empty())
				LOG_WARNING("server_name has more than one value. Setting only the first one.");
			temp = ++it;
			for (; temp != variables.end(); ++temp)
				if (temp->first == "server_name")
					ft_error("server_name was declared more that once. Exiting...", 1);
		}
		else if (it->first == "listen")
		{
			for (std::vector <std::string>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
				separateHostPort(server, *iter);
			}
		}
		else if (it->first == "root")
		{
			defaultLocation.setRoot(extractAndRemoveFirst(it->second));
			if (!it->second.empty())
				LOG_WARNING("root has more than one value. Setting only the first one.");
		}
		else if (it->first == "index")
		{
			for (std::vector <std::string>::iterator iter = it->second.begin(); iter != it->second.end(); iter = it->second.erase(iter)){
				defaultLocation.addIndex(*iter);
			}
		}
		else if (it->first == "method")
		{
			for (std::vector <std::string>::iterator iter = it->second.begin(); iter != it->second.end(); iter = it->second.erase(iter)){
				defaultLocation.addMethod(*iter);
			}
		}
		else if (it->first == "directory_listing")
		{
			std::vector <std::string>::iterator iter = it->second.begin();
			defaultLocation.setDirectoryListingEnabled(*iter);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("directory_listing has more than one value. Setting only the first one.");
		}
		else if (it->first == "directory_response_file")
		{
			std::vector <std::string>::iterator iter = it->second.begin();
			defaultLocation.setDirectoryResponseFile(*iter);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("directory_listing has more than one value. Setting only the first one.");
		}
		else if (it->first == "cgi_enabled")
		{
			std::vector <std::string>::iterator iter = it->second.begin();
			defaultLocation.setCgiEnabled(*iter);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("directory_listing has more than one value. Setting only the first one.");
		}
		else if (it->first == "cgi")
		{
			if (it->second.size() != 2){
				LOG_ERROR("Cgi syntax error. Needs 2 arguments: extension, path");
				exit(1);
			}
			std::vector <std::string>::iterator iter = it->second.begin();
			std::string extension = *iter;
			iter = it->second.erase(iter);
			defaultLocation.addCgi(extension, *iter);
		}
		else if (it->first == "upload_enabled")
		{
			std::vector <std::string>::iterator iter = it->second.begin();
			defaultLocation.setUploadEnabled(*iter);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("upload_enabled has more than one value. Setting only the first one.");
		}
		else if (it->first == "upload_path")
		{
			std::vector <std::string>::iterator iter = it->second.begin();
			defaultLocation.setUploadPath(*iter);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("upload_path has more than one value. Setting only the first one.");
		}
		else if (it->first == "redirect")
		{
			std::vector <std::string>::iterator iter = it->second.begin();
			defaultLocation.setRedirect(*iter);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("redirect has more than one value. Setting only the first one.");
		}
		else if (it->first == "error_page")
		{
			int code = 0;
			if (it->second.size() != 2){
				LOG_ERROR("error_page syntax error. Needs 2 arguments: extension, path");
				exit(1);
			}
			std::vector <std::string>::iterator iter = it->second.begin();
			try {
				code = stoi(*iter);
			} catch (std::exception &e) { LOG_ERROR("Error code not valid!"); exit(1); }
			iter = it->second.erase(iter);
			defaultLocation.addErrorPage(code, *iter);
		}
		else if (it->first == "clientMaxBodySize")
		{
			int size = 0;
			std::vector <std::string>::iterator iter = it->second.begin();
			try {
				size = stoul(*iter);
			} catch (std::exception &e) { LOG_ERROR("Max body size not valid!"); exit(1); }
			defaultLocation.setClientMaxBodySize(size);
			iter = it->second.erase(iter);
			if (!it->second.empty())
				LOG_WARNING("clientMaxBodySize has more than one value. Setting only the first one.");
		}
		else
			ft_error("Unkown variable \"" + it->first +  "\", exiting...", 1);
		for (std::vector <std::string>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
			
		}
	}
	server.setDefaultLocation(defaultLocation);
	_servers.push_back(server);
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

void ParseConfig::printAll()
{
	for (std::vector<ServerConfig>::iterator serverIt = _servers.begin(); serverIt != _servers.end(); ++serverIt) {
        ServerConfig& server = *serverIt;

        std::cout << "Server Name: " << server.getServerName() << std::endl;

        // Print Listen Configurations
        const std::vector<std::pair<std::string, int> >& listenConfigurations = server.getListen();
        for (std::vector<std::pair<std::string, int> >::const_iterator listenIt = listenConfigurations.begin(); listenIt != listenConfigurations.end(); ++listenIt) {
            const std::pair<std::string, int>& listen = *listenIt;
            std::cout << "Listen: " << listen.first << ":" << listen.second << std::endl;
        }

        // Print Default Location
        const LocationConfig& defaultLocation = server.getDefaultLocation();
        std::cout << "Default Location:" << std::endl;
        std::cout << "  Path: " << defaultLocation.getPath() << std::endl;
        std::cout << "  Root: " << defaultLocation.getRoot() << std::endl;

        // Print Locations
        const std::vector<LocationConfig>& locations = server.getLocations();
        for (std::vector<LocationConfig>::const_iterator locationIt = locations.begin(); locationIt != locations.end(); ++locationIt) {
            const LocationConfig& location = *locationIt;

            std::cout << "Location:" << std::endl;
            std::cout << "  Path: " << location.getPath() << std::endl;
            std::cout << "  Root: " << location.getRoot() << std::endl;

            // Print Location Details (add more details as needed)
            std::cout << "  Indexes: ";
            const std::vector<std::string>& indexes = location.getIndexes();
            for (std::vector<std::string>::const_iterator indexIt = indexes.begin(); indexIt != indexes.end(); ++indexIt) {
                const std::string& index = *indexIt;
                std::cout << index << " ";
            }
            std::cout << std::endl;

            std::cout << "  Methods: ";
            const std::vector<std::string>& methods = location.getMethods();
            for (std::vector<std::string>::const_iterator methodIt = methods.begin(); methodIt != methods.end(); ++methodIt) {
                const std::string& method = *methodIt;
                std::cout << method << " ";
            }
            std::cout << std::endl;

            // Add more details if needed
        }

        std::cout << std::endl;  // Add a separator between servers
    }
}