//
// Created by kolterdyx on 14/09/23.
//

#ifndef WEBSERV_LOCATIONCONFIG_HPP
#define WEBSERV_LOCATIONCONFIG_HPP

#include <string>
#include <vector>

class LocationConfig {

private:
    std::string path;
    std::string root;
    std::vector<std::string> indexes;
    std::vector<std::string> methods;

    bool directoryListingEnabled;
    std::string directoryResponseFile;

    bool cgiEnabled;
    std::string cgiPath;
    std::string cgiExtension;

    bool uploadEnabled;
    std::string uploadPath;

public:

    LocationConfig();
    LocationConfig(const LocationConfig& other);
    LocationConfig& operator=(const LocationConfig& other);
    ~LocationConfig();

    LocationConfig addIndex(const std::string& index);

    LocationConfig addMethod(const std::string& method);

    LocationConfig setPath(const std::string &path);

    LocationConfig setRoot(const std::string &root);

    LocationConfig setDirectoryListingEnabled(bool directoryListingEnabled);

    LocationConfig setDirectoryResponseFile(const std::string &directoryResponseFile);

    LocationConfig setCgiEnabled(bool cgiEnabled);

    LocationConfig setCgiPath(const std::string &cgiPath);

    LocationConfig setCgiExtension(const std::string &cgiExtension);

    LocationConfig setUploadEnabled(bool uploadEnabled);

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
