//
// Created by kolterdyx on 14/09/23.
//

#include "LocationConfig.hpp"

LocationConfig::LocationConfig() : directoryListingEnabled(), cgiEnabled(), uploadEnabled() {

}

LocationConfig::~LocationConfig() {

}

void LocationConfig::setPath(const std::string &path) {
    this->path = path;
}

void LocationConfig::setRoot(const std::string &root) {
    this->root = root;
}

void LocationConfig::setDirectoryListingEnabled(bool directoryListingEnabled) {
    this->directoryListingEnabled = directoryListingEnabled;
}

void LocationConfig::setDirectoryResponseFile(const std::string &directoryResponseFile) {
    this->directoryResponseFile = directoryResponseFile;
}

void LocationConfig::setCgiEnabled(bool cgiEnabled) {
    this->cgiEnabled = cgiEnabled;
}

void LocationConfig::setCgiPath(const std::string &cgiPath) {
    this->cgiPath = cgiPath;
}

void LocationConfig::setCgiExtension(const std::string &cgiExtension) {
    this->cgiExtension = cgiExtension;
}

void LocationConfig::setUploadEnabled(bool uploadEnabled) {
    this->uploadEnabled = uploadEnabled;
}

void LocationConfig::setUploadPath(const std::string &uploadPath) {
    this->uploadPath = uploadPath;
}

void LocationConfig::addIndex(const std::string &index) {
    indexes.push_back(index);
}

void LocationConfig::addMethod(const std::string &method) {
    methods.push_back(method);
}

const std::string &LocationConfig::getPath() const {
    return path;
}

const std::string &LocationConfig::getRoot() const {
    return root;
}

const std::vector<std::string> &LocationConfig::getIndexes() const {
    return indexes;
}

const std::vector<std::string> &LocationConfig::getMethods() const {
    return methods;
}

bool LocationConfig::isDirectoryListingEnabled() const {
    return directoryListingEnabled;
}

const std::string &LocationConfig::getDirectoryResponseFile() const {
    return directoryResponseFile;
}

bool LocationConfig::isCgiEnabled() const {
    return cgiEnabled;
}

const std::string &LocationConfig::getCgiPath() const {
    return cgiPath;
}

const std::string &LocationConfig::getCgiExtension() const {
    return cgiExtension;
}

bool LocationConfig::isUploadEnabled() const {
    return uploadEnabled;
}

const std::string &LocationConfig::getUploadPath() const {
    return uploadPath;
}
