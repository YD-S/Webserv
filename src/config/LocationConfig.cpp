//
// Created by kolterdyx on 14/09/23.
//
#include "../includes/config/LocationConfig.hpp"

LocationConfig::LocationConfig() : _directoryListingEnabled(), _cgiEnabled(), _uploadEnabled() {

}

LocationConfig::LocationConfig(const LocationConfig &other) : _directoryListingEnabled(), _cgiEnabled(), _uploadEnabled() {
    *this = other;
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other) {
    if (this != &other) {
        this->_path = other._path;
        this->_root = other._root;
        this->_indexes = other._indexes;
        this->_methods = other._methods;
        this->_directoryListingEnabled = other._directoryListingEnabled;
        this->_directoryResponseFile = other._directoryResponseFile;
        this->_cgiEnabled = other._cgiEnabled;
        this->_cgiPath.clear();
		for (std::vector<std::string>::const_iterator it = other._cgiPath.begin(); it != other._cgiPath.end(); ++it){
			this->_cgiPath.push_back(*it);
		}
		this->_cgiExtension.clear();
		for (std::vector<std::string>::const_iterator it = other._cgiExtension.begin(); it != other._cgiExtension.end(); ++it){
			this->_cgiExtension.push_back(*it);
		}
        this->_uploadEnabled = other._uploadEnabled;
        this->_uploadPath = other._uploadPath;
    }
    return *this;
}

LocationConfig::~LocationConfig() {

}





void LocationConfig::setDirectoryListingEnabled(const std::string &boolean){
	if (boolean == "on")
		this->_directoryListingEnabled = true;
	else if (boolean == "off")
		this->_directoryListingEnabled = false;
	else
		LOG_ERROR("directoryListingEnabled has unkown bool type " + boolean);
}

void LocationConfig::setCgiEnabled(const std::string &boolean){
	if (boolean == "on")
		this->_cgiEnabled = true;
	else if (boolean == "off")
		this->_cgiEnabled = false;
	else
		LOG_ERROR("cgiEnabled has unkown bool type " + boolean);
}

void LocationConfig::setUploadEnabled(const std::string &boolean){
	if (boolean == "on")
		this->_uploadEnabled = true;
	else if (boolean == "off")
		this->_uploadEnabled = false;
	else
		LOG_ERROR("uploadEnabled has unkown bool type " + boolean);
}

void LocationConfig::setPath(const std::string &path) {
    this->_path = path;
}

void LocationConfig::setRoot(const std::string &root) {
    this->_root = root;
}

void LocationConfig::setDirectoryListingEnabled(bool directoryListingEnabled) {
    this->_directoryListingEnabled = directoryListingEnabled;
}

void LocationConfig::setDirectoryResponseFile(const std::string &directoryResponseFile) {
    this->_directoryResponseFile = directoryResponseFile;
}

void LocationConfig::setCgiEnabled(bool cgiEnabled) {
    this->_cgiEnabled = cgiEnabled;
}

void LocationConfig::setUploadEnabled(bool uploadEnabled) {
    this->_uploadEnabled = uploadEnabled;
}

void LocationConfig::setUploadPath(const std::string &uploadPath) {
    this->_uploadPath = uploadPath;
}

void LocationConfig::addIndex(const std::string &index) {
    _indexes.push_back(index);
}

void LocationConfig::addMethod(const std::string &method) {
    _methods.push_back(method);
}

const std::string &LocationConfig::getPath() const {
    return _path;
}

const std::string &LocationConfig::getRoot() const {
    return _root;
}

const std::vector<std::string> &LocationConfig::getIndexes() const {
    return _indexes;
}

const std::vector<std::string> &LocationConfig::getMethods() const {
    return _methods;
}

bool LocationConfig::isDirectoryListingEnabled() const {
    return _directoryListingEnabled;
}

const std::string &LocationConfig::getDirectoryResponseFile() const {
    return _directoryResponseFile;
}

bool LocationConfig::isCgiEnabled() const {
    return _cgiEnabled;
}

bool LocationConfig::isUploadEnabled() const {
    return _uploadEnabled;
}

const std::string &LocationConfig::getUploadPath() const {
    return _uploadPath;
}

const std::vector<std::string>& LocationConfig::getCgiPath() const {
    return _cgiPath;
}

void LocationConfig::addCgiPath(const std::string& cgiPath) {
	_cgiPath.push_back(cgiPath);
}

const std::vector<std::string>& LocationConfig::getCgiExtension() const {
	return _cgiExtension;
}

void LocationConfig::addCgiExtension(const std::string& cgiExtension) {
	_cgiExtension.push_back(cgiExtension);
}
