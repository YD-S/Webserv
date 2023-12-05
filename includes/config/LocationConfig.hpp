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
    std::vector<std::string> _cgiPath;
    std::vector<std::string> _cgiExtension;

    bool _uploadEnabled;
    std::string _uploadPath;

	std::string _redirect;

public:

    LocationConfig();
    LocationConfig(const LocationConfig& other);
    LocationConfig& operator=(const LocationConfig& other);
    ~LocationConfig();

    void addFunctions(std::unordered_map<std::string, void (LocationConfig::*)(const std::string&)> &functionMap);
	void parseLocation(std::vector<std::pair<std::string, std::string> >::iterator &it, std::vector<std::pair<std::string, std::string> > &config);

    void addIndex(const std::string& index);

    void addMethod(const std::string& method);

	void setPath(const std::string &path);

    void setRoot(const std::string &root);

    void setDirectoryListingEnabled(bool directoryListingEnabled);

	void setDirectoryListingEnabled(const std::string &directoryListingEnabled);

    void setDirectoryResponseFile(const std::string &directoryResponseFile);

    void setCgiEnabled(bool cgiEnabled);
    
    void setCgiEnabled(const std::string &boolean);

    void setUploadEnabled(bool uploadEnabled);

	void setUploadEnabled(const std::string &uploadEnabled);

    void setUploadPath(const std::string &uploadPath);

    const std::string &getPath() const;

    const std::string &getRoot() const;

    const std::vector<std::string> &getIndexes() const;

    const std::vector<std::string> &getMethods() const;

    bool isDirectoryListingEnabled() const;

    const std::string &getDirectoryResponseFile() const;

    bool isCgiEnabled() const;

    bool isUploadEnabled() const;

    const std::string &getUploadPath() const;

    const std::vector<std::string>& getCgiPath() const;

    // Setter for _cgiPath
    void addCgiPath(const std::string& cgiPath);

    // Getter for _cgiExtension
    const std::vector<std::string>& getCgiExtension() const;

    // Setter for _cgiExtension
    void addCgiExtension(const std::string& cgiExtension);
};


#endif //WEBSERV_LOCATIONCONFIG_HPP
