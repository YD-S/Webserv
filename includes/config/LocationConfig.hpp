//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_LOCATIONCONFIG_HPP
#define WEBSERV_LOCATIONCONFIG_HPP

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../utils.hpp"
#include "macros.h"

class LocationConfig {

private:

	std::string _path;
	std::string _root;
	std::vector<std::string> _indexes;
	bool _autoIndex;
	std::vector<std::string> _methods;

	bool _directoryListingEnabled;
	std::string _directoryResponseFile;

	bool _cgiEnabled;
	std::vector<std::pair<std::string, std::string> > _cgi;

	bool _uploadEnabled;
	std::string _uploadPath;

	std::string _redirect;
	std::vector<std::pair<int, std::string> > _errorPages;

	size_t _clientMaxBodySize;

public:

	LocationConfig();

	LocationConfig(const LocationConfig &other);

	LocationConfig &operator=(const LocationConfig &other);

	~LocationConfig();

	void setDefaultConfig(LocationConfig *defaultLocation);

	void addIndex(const std::string &index);

	void setAutoIndex(bool autoIndex);

	void addMethod(const std::string &method);

	void setPath(const std::string &path);

	void setRoot(const std::string &root);

	void setDirectoryListingEnabled(bool directoryListingEnabled);

	void setDirectoryListingEnabled(const std::string &directoryListingEnabled);

	void setDirectoryResponseFile(const std::string &directoryResponseFile);

	void addCgi(const std::string &extension, const std::string &path);

	void setCgiEnabled(bool cgiEnabled);

	void setCgiEnabled(const std::string &boolean);

	void setUploadEnabled(bool uploadEnabled);

	void setUploadEnabled(const std::string &uploadEnabled);

	void setUploadPath(const std::string &uploadPath);

	void addErrorPage(const int &code, const std::string &path);

	void setClientMaxBodySize(size_t size);

	void setRedirect(const std::string &redirect);

	const std::string &getPath() const;

	const std::string &getRoot() const;

	const std::vector<std::string> &getIndexes() const;

	const std::vector<std::string> &getMethods() const;

	bool isDirectoryListingEnabled() const;

	const std::string &getDirectoryResponseFile() const;

	bool isCgiEnabled() const;

	bool isUploadEnabled() const;

	const std::string &getUploadPath() const;

	const std::vector<std::pair<std::string, std::string> > &getCgi() const;

	const std::vector<std::pair<int, std::string> > &getErrorPages() const;

	bool isAutoIndexEnabled() const;

	const std::string &getRedirect() const;

	void clearIndex();

	bool hasMethod(std::string method) const;

	std::string toString();

	std::string getErrorPage(int statusCode);
};


#endif //WEBSERV_LOCATIONCONFIG_HPP
