//
// Created by kolterdyx on 5/10/23.
//

#ifndef WEBSERV_ICGIEXECUTOR_HPP
#define WEBSERV_ICGIEXECUTOR_HPP


#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

class ICgiExecutor {

private:
	std::string _cgiPath;
	std::string _cgiName;

protected:
	std::string _cgiResult;

	const LocationConfig *_config;

	char **envp;

public:
	ICgiExecutor();

	ICgiExecutor(const std::string &cgiPath, const std::string &cgiName, char **envp, const LocationConfig *location);

	virtual ~ICgiExecutor() = 0;

	const std::string &getCgiPath() const;

	void setCgiPath(const std::string &cgiPath);

	const std::string &getCgiName() const;

	void setCgiName(const std::string &cgiName);


#if DEBUG == 0

	const std::string &getCgiResult() const;

#endif

	/**
	 * Passes a request to the CGI and returns the response via parameter
	 * @param request HttpRequest object reference to be passed to CGI
	 * @param response HttpResponse object reference to be filled with CGI response
	 */
	virtual bool executeCgi(HttpRequest *request, std::string *response, std::string scriptPath) = 0;


};


#endif //WEBSERV_ICGIEXECUTOR_HPP
