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
#include <unordered_map>
#include "macros.h"

class LocationConfig {

private:
    std::string _path;
    std::string _root;
    std::vector<std::string> _indexes;
    std::vector<std::string> _methods;

    bool _directoryListingEnabled;
    std::string _directoryResponseFile;

    bool _cgiEnabled;
    std::string _cgiPath;
    std::string _cgiExtension;

    bool _uploadEnabled;
    std::string _uploadPath;

	std::string _redirect;

public:

    LocationConfig();
    LocationConfig(const LocationConfig& other);
    LocationConfig& operator=(const LocationConfig& other);
    ~LocationConfig();

	void parseLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config);

	void addFunctions(std::unordered_map<std::string, LocationConfig (LocationConfig::*)(const std::string&)> &functionMap);

    LocationConfig addIndex(const std::string& index);

    LocationConfig addMethod(const std::string& method);

	LocationConfig setPath(const std::string &path);

    LocationConfig setRoot(const std::string &root);

    LocationConfig setDirectoryListingEnabled(bool directoryListingEnabled);

	LocationConfig setDirectoryListingEnabled(const std::string &directoryListingEnabled);

    LocationConfig setDirectoryResponseFile(const std::string &directoryResponseFile);

    LocationConfig setCgiEnabled(bool cgiEnabled);

	LocationConfig setCgiEnabled(const std::string &cgiEnabled);

    LocationConfig setCgiPath(const std::string &cgiPath);

    LocationConfig setCgiExtension(const std::string &cgiExtension);

    LocationConfig setUploadEnabled(bool uploadEnabled);

	LocationConfig setUploadEnabled(const std::string &uploadEnabled);

    LocationConfig setUploadPath(const std::string &uploadPath);

    const std::string &getPath() const;

    const std::string &getRoot() const;

    const std::vector<std::string> &getIndexes() const;

    const std::vector<std::string> &getMethods() const;

    bool isDirectoryListingEnabled() const;

    const std::string &getDirectoryResponseFile() const;

    bool isCgiEnabled() const;

    const std::string &getCgiPath() const;

    const std::string &getCgiExtension() const;

    bool isUploadEnabled() const;

    const std::string &getUploadPath() const;

};


#endif //WEBSERV_LOCATIONCONFIG_HPP
