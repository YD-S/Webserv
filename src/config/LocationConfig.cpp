//
// Created by kolterdyx on 14/09/23.
//
#include "config/LocationConfig.hpp"

LocationConfig::LocationConfig() : _autoIndex(false), _directoryListingEnabled(false), _cgiEnabled(false), _uploadEnabled(false), _clientMaxBodySize(10000) {
    
}

LocationConfig::LocationConfig(const LocationConfig &other) {
    *this = other;
}

LocationConfig &LocationConfig::operator=(const LocationConfig &other) {
    if (this != &other) {
        this->_path = other._path;
        this->_root = other._root;
        _indexes.clear();
        for (std::vector<std::string>::const_iterator it = other.getIndexes().begin(); it != other.getIndexes().end(); ++it)
            this->addIndex(*it);
        this->_autoIndex = other._autoIndex;
        _methods.clear();
        for (std::vector<std::string>::const_iterator it = other.getMethods().begin(); it != other.getMethods().end(); ++it)
            this->addMethod(*it);
        this->_directoryListingEnabled = other._directoryListingEnabled;
        this->_directoryResponseFile = other._directoryResponseFile;
        this->_cgiEnabled = other._cgiEnabled;
        _cgi.clear();
        for (std::vector<std::pair<std::string, std::string> >::const_iterator it = other.getCgi().begin(); it != other.getCgi().end(); ++it)
            this->addCgi(it->first, it->second);
        this->_uploadEnabled = other._uploadEnabled;
        this->_uploadPath = other._uploadPath;
        this->_redirect = other._redirect;
        _errorPages.clear();
        for (std::vector<std::pair<int, std::string> >::const_iterator it = other.getErrorPages().begin(); it != other.getErrorPages().end(); ++it)
            this->addErrorPage(it->first, it->second);
        this->_clientMaxBodySize = other._clientMaxBodySize;
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
		LOG_SYS_ERROR("directoryListingEnabled has unkown bool type " + boolean);
}

void LocationConfig::setCgiEnabled(const std::string &boolean){
	if (boolean == "on")
		this->_cgiEnabled = true;
	else if (boolean == "off")
		this->_cgiEnabled = false;
	else
		LOG_SYS_ERROR("cgiEnabled has unkown bool type " + boolean);
}

void LocationConfig::setUploadEnabled(const std::string &boolean){
	if (boolean == "on")
		this->_uploadEnabled = true;
	else if (boolean == "off")
		this->_uploadEnabled = false;
	else
		LOG_SYS_ERROR("uploadEnabled has unkown bool type " + boolean);
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

void LocationConfig::addCgi(const std::string &extension, const std::string &path){
    _cgi.push_back(std::make_pair(extension, path));
}

void LocationConfig::addErrorPage(const int &code, const std::string &path){
    _errorPages.push_back(std::make_pair(code, path));
}

void LocationConfig::setClientMaxBodySize(const size_t size){
    _clientMaxBodySize = size;
}

void LocationConfig::setRedirect(const std::string &redirect){
    _redirect = redirect;
}

const std::vector<std::pair<std::string, std::string> >& LocationConfig::getCgi() const{
    return _cgi;
}

const std::vector<std::pair<int, std::string> > &LocationConfig::getErrorPages() const{
    return _errorPages;
}

void LocationConfig::setAutoIndex(bool autoIndex){
    _autoIndex = autoIndex;
}

bool LocationConfig::isAutoIndexEnabled() const{
    return _autoIndex;
}

const std::string &LocationConfig::getRedirect() const{
    return _redirect;
}

void  LocationConfig::clearIndex(){
    _indexes.clear();
}

bool LocationConfig::hasMethod(std::string method) const{
	for(std::vector<std::string>::const_iterator it = _methods.begin(); it != _methods.end(); ++it){
		if (method == *it)
			return true;
	}
	return false;
}

std::string LocationConfig::toString() {
	std::string result = "";
	result += "path: " + _path + "\n";
	result += "root: " + _root + "\n";
	result += "indexes: ";
	for (std::vector<std::string>::const_iterator it = _indexes.begin(); it != _indexes.end(); ++it) {
		result += *it + " ";
	}
	result += "\n";
	result += "methods: ";
	for (std::vector<std::string>::const_iterator it = _methods.begin(); it != _methods.end(); ++it) {
		result += *it + " ";
	}
	result += "\n";
	result += "directoryListingEnabled: " + to_string(_directoryListingEnabled) + "\n";
	result += "directoryResponseFile: " + _directoryResponseFile + "\n";
	result += "cgiEnabled: " + to_string(_cgiEnabled) + "\n";
	result += "uploadEnabled: " + to_string(_uploadEnabled) + "\n";
	result += "uploadPath: " + _uploadPath + "\n";
	result += "cgi: ";
	for (std::vector<std::pair<std::string, std::string> >::const_iterator it = _cgi.begin(); it != _cgi.end(); ++it) {
		result += it->first + " " + it->second + " ";
	}
	result += "\n";
	result += "errorPages: ";
	for (std::vector<std::pair<int, std::string> >::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it) {
		result += to_string(it->first) + " " + it->second + " ";
	}
	result += "\n";
	result += "clientMaxBodySize: " + to_string(_clientMaxBodySize) + "\n";
	result += "redirect: " + _redirect + "\n";
	return result;
}

std::string LocationConfig::getErrorPage(const int statusCode) {
	for (std::vector<std::pair<int, std::string> >::const_iterator it = _errorPages.begin(); it != _errorPages.end(); ++it) {
		if (it->first == statusCode)
			return it->second;
	}
	return "";
}
