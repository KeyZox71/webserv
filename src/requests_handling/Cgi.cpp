/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/19 11:41:28 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <help.hpp>
#include <requests/Cgi.hpp>

#include <fcntl.h>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using namespace webserv;

// WARN: construtor will probably be changed and practicly do nothing
Cgi::Cgi(http::ARequest *req, config::Route *conf)
	: _conf(conf), _request(req) {
	_initEnvp();
	_cgi_path = _conf->getCgiPath(req->getTarget());
	if (_cgi_path == "") {
		// TODO: need to make something
	}
}

void Cgi::_initEnvp(void) {
	std::stringstream str;
	str << WEBSRV_NAME << "/" << WEBSRV_VERSION;
	setEnv("SERVER_SOFTWARE", str.str());
	str.clear();
	setEnv("SERVER_NAME", _request->getHeader("Host"));
	setEnv("SERVER_PROTOCOL", _request->getProtocol());
	// setEnv("SERVER_PORT", _request->get); // TODO: need to get the port by a
	// way i dont know yet

	setEnv("GATEWAY_INTERFACE", "CGI/1.1");

	// setEnv("PATH_TRANSLATED", ); // TODO: wtf should i put here i dont fcking
	// know
	// setEnv("PATH_INFO", ); // TODO: wut make no sense

	str << _request->getBody().length();
	setEnv("CONTENT_LENGH", str.str());
	str.clear();
	setEnv("CONTENT_TYPE", _request->getHeader("Content-Type"));
	// setEnv("REMOTE_ADDR", _request->get) // TODO: don't have it yet need to
	// be passed to the requset :sob:
	setEnv("HTTP_ACCEPT", _request->getHeader("Accept"));
	setEnv("HTTP_ACCEPT_LANGUAGE", _request->getHeader("Accept-Language"));
	setEnv("HTTP_COOKIE", _request->getHeader("Cookie"));
	setEnv("HTTP_HOST", _request->getHeader("Host"));
	setEnv("HTTP_REFERER", _request->getHeader("Referer"));
	setEnv("HTTP_USER_AGENT", _request->getHeader("User-Agent"));

	setEnv("SCRIPT_NAME", _request->getTarget());

	setEnv("QUERY_STRING", _request->getUrl().getQueryString());
}

std::string Cgi::getEnv(std::string &key) {
	auto it = _envp.find(key);
	if (it != _envp.end()) {
		return it->second;
	}
	return "";
}

void Cgi::setEnv(const std::string key, std::string value) {
	_envp[key] = value;
}

char **Cgi::_genEnv(void) {
	char **newEnv = new char *[_envp.size() + 1];
	size_t i = 0;

	for (auto it = range(_envp), i++) {
		std::string str = it->first + "=" + it->second;
		char	   *tmp = new char[str.size() + 1];
		std::strcpy(tmp, str.c_str());
		newEnv[i] = tmp;
	}

	return newEnv;
}

void Cgi::process(void) {
	int	  pipefd[2];
	pid_t forkPid;

	if (pipe(pipefd) == -1) {
		throw;
		// TODO: error handling pipe fail
	}

	forkPid = fork();
	if (forkPid < 0) {
		throw;
		// TODO: fork fail
	} else if (forkPid == 0) {
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		close(pipefd[0]);

		char  *argv[] = {const_cast<char *>(_cgi_path.c_str()),
						 const_cast<char *>(_script_path.c_str()), NULL};
		char **env = _genEnv();

		if (execve(_cgi_path.c_str(), argv, env) == -1) {
			std::stringstream str;
			str << "how did you do that ???? : ";
			str << errno;
			_log->error(str.str());
			for (int i = 0; env[i] != NULL; i++)
				delete env[i];
			delete env;
			exit(EXIT_FAILURE);
		}
	}
	waitpid(forkPid, NULL, 0);
}
