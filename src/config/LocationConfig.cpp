//
// Created by kolterdyx on 14/09/23.
//

#include "config/LocationConfig.hpp"

LocationConfig::LocationConfig() : directoryListingEnabled(), cgiEnabled(), uploadEnabled() {

}

LocationConfig::LocationConfig(const LocationConfig &other) : directoryListingEnabled(), cgiEnabled(), uploadEnabled() {
    *this = other;
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other) {
    if (this != &other) {
        this->path = other.path;
        this->root = other.root;
        this->indexes = other.indexes;
        this->methods = other.methods;
        this->directoryListingEnabled = other.directoryListingEnabled;
        this->directoryResponseFile = other.directoryResponseFile;
        this->cgiEnabled = other.cgiEnabled;
        this->cgiPath = other.cgiPath;
        this->cgiExtension = other.cgiExtension;
        this->uploadEnabled = other.uploadEnabled;
        this->uploadPath = other.uploadPath;
    }
    return *this;
}

LocationConfig::~LocationConfig() {

}

LocationConfig LocationConfig::setPath(const std::string &path) {
    this->path = path;
    return *this;
}

LocationConfig LocationConfig::setRoot(const std::string &root) {
    this->root = root;
    return *this;
}

LocationConfig LocationConfig::setDirectoryListingEnabled(bool directoryListingEnabled) {
    this->directoryListingEnabled = directoryListingEnabled;
    return *this;
}

LocationConfig LocationConfig::setDirectoryResponseFile(const std::string &directoryResponseFile) {
    this->directoryResponseFile = directoryResponseFile;
    return *this;
}

LocationConfig LocationConfig::setCgiEnabled(bool cgiEnabled) {
    this->cgiEnabled = cgiEnabled;
    return *this;
}

LocationConfig LocationConfig::setCgiPath(const std::string &cgiPath) {
    this->cgiPath = cgiPath;
    return *this;
}

LocationConfig LocationConfig::setCgiExtension(const std::string &cgiExtension) {
    this->cgiExtension = cgiExtension;
    return *this;
}

LocationConfig LocationConfig::setUploadEnabled(bool uploadEnabled) {
    this->uploadEnabled = uploadEnabled;
    return *this;
}

LocationConfig LocationConfig::setUploadPath(const std::string &uploadPath) {
    this->uploadPath = uploadPath;
    return *this;
}

LocationConfig LocationConfig::addIndex(const std::string &index) {
    indexes.push_back(index);
    return *this;
}

LocationConfig LocationConfig::addMethod(const std::string &method) {
    methods.push_back(method);
    return *this;
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