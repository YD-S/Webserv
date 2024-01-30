//
// Created by kolterdyx on 5/10/23.
//

#include "ICgiExecutor.hpp"

ICgiExecutor::ICgiExecutor(const std::string &cgiPath, const std::string &cgiName, char **envp) {
	this->_cgiPath = cgiPath;
	this->_cgiName = cgiName;
	this->envp = envp;
}

ICgiExecutor::ICgiExecutor() {

}

ICgiExecutor::~ICgiExecutor() {

}

const std::string &ICgiExecutor::getCgiPath() const {
	return _cgiPath;
}

void ICgiExecutor::setCgiPath(const std::string &cgiPath) {
	_cgiPath = cgiPath;
}

const std::string &ICgiExecutor::getCgiName() const {
	return _cgiName;
}

void ICgiExecutor::setCgiName(const std::string &cgiName) {
	_cgiName = cgiName;
}

#if DEBUG == 0

const std::string &ICgiExecutor::getCgiResult() const {
	return _cgiResult;
}

#endif
