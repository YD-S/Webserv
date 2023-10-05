//
// Created by kolterdyx on 5/10/23.
//

#include "ICgiExecutor.hpp"

ICgiExecutor::ICgiExecutor(const std::string &cgiPath, const std::string &cgiName, const std::string &cgiArgs,
                           const std::string &cgiBody, const std::string &cgiEnv) {
    this->_cgiPath = cgiPath;
    this->_cgiName = cgiName;
    this->_cgiArgs = cgiArgs;
    this->_cgiBody = cgiBody;
    this->_cgiEnv = cgiEnv;
}

ICgiExecutor::ICgiExecutor() {

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

const std::string &ICgiExecutor::getCgiArgs() const {
    return _cgiArgs;
}

void ICgiExecutor::setCgiArgs(const std::string &cgiArgs) {
    _cgiArgs = cgiArgs;
}

const std::string &ICgiExecutor::getCgiBody() const {
    return _cgiBody;
}

void ICgiExecutor::setCgiBody(const std::string &cgiBody) {
    _cgiBody = cgiBody;
}

const std::string &ICgiExecutor::getCgiEnv() const {
    return _cgiEnv;
}

void ICgiExecutor::setCgiEnv(const std::string &cgiEnv) {
    _cgiEnv = cgiEnv;
}

const std::string &ICgiExecutor::getCgiResult() const {
    return _cgiResult;
}

void ICgiExecutor::setCgiResult(const std::string &cgiResult) {
    _cgiResult = cgiResult;
}
