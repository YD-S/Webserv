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
    ~LocationConfig();

    void addIndex(const std::string& index);

    void addMethod(const std::string& method);

    void setPath(const std::string &path);

    void setRoot(const std::string &root);

    void setDirectoryListingEnabled(bool directoryListingEnabled);

    void setDirectoryResponseFile(const std::string &directoryResponseFile);

    void setCgiEnabled(bool cgiEnabled);

    void setCgiPath(const std::string &cgiPath);

    void setCgiExtension(const std::string &cgiExtension);

    void setUploadEnabled(bool uploadEnabled);

    void setUploadPath(const std::string &uploadPath);

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
