/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/23 18:26:45 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <help.hpp>
#include <requests/default.hpp>
#include <server/default.hpp>

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace webserv::server;

// WARN: construtor will probably be changed and practicly do nothing
Cgi::Cgi(http::Get *req, config::Route *conf, int id)
	: _prepared(false), _executed(false), _conf(conf), _request(req) {
	_initEnvp();
	_res_id = id;
	_cgi_path = _conf->getCgiPath(req->getTarget());
	if (_cgi_path == "") {
		throw;
		// TODO: need to make something probably will be checked before by
		// client
	}
}

Cgi::Cgi(http::Post *req, config::Route *conf, int id)
	: _prepared(false), _executed(false), _conf(conf), _request(req) {
	_initEnvp();
	_res_id = id;
	_cgi_path = _conf->getCgiPath(req->getTarget());
	if (_cgi_path == "") {
		throw;
		// TODO: need to make something probably will be checked before by
		// client
	}
}

Cgi::~Cgi(void) {

}

void Cgi::_initEnvp(void) {
	std::stringstream str;
	str << WEBSRV_NAME << "/" << WEBSRV_VERSION;
	_setEnv("SERVER_SOFTWARE", str.str());
	str.clear();
	_setEnv("SERVER_NAME", _request->getHeader("Host"));
	_setEnv("SERVER_PROTOCOL", _request->getProtocol());
	// setEnv("SERVER_PORT", _request->get); // TODO: need to get the port by a
	// way i dont know yet

	_setEnv("GATEWAY_INTERFACE", "CGI/1.1");

	// setEnv("PATH_TRANSLATED", ); // TODO: wtf should i put here i dont fcking
	// know
	// setEnv("PATH_INFO", ); // TODO: wut make no sense

	str << _request->getBody().length();
	_setEnv("CONTENT_LENGH", str.str());
	str.clear();
	_setEnv("CONTENT_TYPE", _request->getHeader("Content-Type"));
	// setEnv("REMOTE_ADDR", _request->get) // TODO: don't have it yet need to
	// be passed to the requset :sob:
	_setEnv("HTTP_ACCEPT", _request->getHeader("Accept"));
	_setEnv("HTTP_ACCEPT_LANGUAGE", _request->getHeader("Accept-Language"));
	_setEnv("HTTP_COOKIE", _request->getHeader("Cookie"));
	_setEnv("HTTP_HOST", _request->getHeader("Host"));
	_setEnv("HTTP_REFERER", _request->getHeader("Referer"));
	_setEnv("HTTP_USER_AGENT", _request->getHeader("User-Agent"));

	_setEnv("SCRIPT_NAME", _request->getTarget());

	_setEnv("QUERY_STRING", _request->getUrl().getQueryString());
}

std::string Cgi::_getEnv(std::string &key) const {
	auto it = _envp.find(key);
	if (it != _envp.end()) {
		return it->second;
	}
	return "";
}

void Cgi::_setEnv(const std::string key, std::string value) {
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

void Cgi::prepare(void) {
	if (pipe(_stdin_pipe) == -1 && pipe(_stdout_pipe) == -1) {
		throw;
		// TODO: need to make a better throw
	}
	_fd->fd = _stdin_pipe[1];
	_fd->events = POLLOUT;
	_prepared = true;
}

void Cgi::process(void) {
	pid_t forkPid;

	forkPid = fork();
	if (forkPid < 0) {
		throw;
		// TODO: fork fail
	} else if (forkPid == 0) {
		dup2(_stdin_pipe[0], STDIN_FILENO);
		close(_stdin_pipe[0]);
		close(_stdin_pipe[1]);

		dup2(_stdout_pipe[1], STDOUT_FILENO);
		close(_stdout_pipe[0]);
		close(_stdout_pipe[1]);

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
	close(_stdin_pipe[0]);
	close(_stdout_pipe[1]);
	waitpid(forkPid, NULL, 0);
	_executed = true;
}

std::string Cgi::str(void) {
	std::string str;
	int			max = _conf->getMaxBody();
	char		buffer[1024];

	while (max) {
		ssize_t count = read(_stdout_pipe[0], buffer, sizeof(buffer));
		if (count > 0)
			str.append(buffer);
		else
			break;
	}
	str.append("\0");
	return str;
}
