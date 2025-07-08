/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/07/08 15:45:20 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/PfdManager.hpp"
#include "server/ResourceManager.hpp"
#include <ctime>
#include <help.hpp>
#include <ios>
#include <requests/default.hpp>
#include <server/default.hpp>

#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

using namespace webserv::server;

Cgi::Cgi(http::Get *req, config::Route *conf)
	: _is_post(false), _conf(conf), _request(req) {
	_processed = false;
	log("➕", "Cgi", "GET constructor called");
	_initEnvp();
	_prep();
}

Cgi::Cgi(http::Post *req, config::Route *conf)
	: _is_post(true), _conf(conf), _request(req) {
	_processed = false;
	log("➕", "Cgi", "POST constructor called");
	_initEnvp();
	_prep();
	AClientResource *in =
		new CgiIn(_request->getBody(), _stdin_pipe[PIPE_WRITE]);
	ResourceManager::append(in);
	struct pollfd pfd;
	pfd.events = in->event();
	pfd.revents = 0;
	pfd.fd = in->getId();
	server::PfdManager::append(pfd, server::RES);
}

Cgi::~Cgi(void) {
	log("➖", "Cgi", "destructor called");
	if (_is_post) {
		PfdManager::remove(_stdin_pipe[PIPE_WRITE]);
		ResourceManager::remove(_stdin_pipe[PIPE_WRITE]);
	}
}

void Cgi::_prep(void) {
	if (_is_post)
		if (pipe(_stdin_pipe) == -1)
			throw std::runtime_error("stdin pipe failed for cgi D:");
	if (pipe(_stdout_pipe) == -1)
		throw std::runtime_error("stdout pipe failed for cgi D:");
	_script_path = _conf->getRootDir() + _request->getTarget();
	_fd = _stdout_pipe[PIPE_READ];
	_pfd_event = POLLIN;
	if (access(_script_path.c_str(), X_OK))
		throw std::runtime_error(
			"script is not executable please run : chmod +x " + _script_path + ", or create the file");
}

void Cgi::_initEnvp(void) {
	std::stringstream str;
	str << WEBSRV_NAME << "/" << WEBSRV_VERSION;
	_setEnv("SERVER_SOFTWARE", str.str());
	str.clear();
	_setEnv("SERVER_NAME", _request->getHeader("Host"));
	_setEnv("SERVER_PROTOCOL", _request->getProtocol());
	str << _request->getServer()->getPort();
	_setEnv("SERVER_PORT", str.str());
	str.clear();

	_setEnv("REQUEST_METHOD", _request->getMethod());

	_setEnv("GATEWAY_INTERFACE", "CGI/1.1");

	_setEnv("PATH_TRANSLATED", _request->getTarget());
	_setEnv("PATH_INFO", _request->getTarget());

	str << _request->getBody().length();
	_setEnv("CONTENT_LENGH", str.str());
	str.clear();
	_setEnv("CONTENT_TYPE", _request->getHeader("Content-Type"));
	_setEnv("HTTP_ACCEPT", _request->getHeader("Accept"));
	_setEnv("HTTP_ACCEPT_LANGUAGE", _request->getHeader("Accept-Language"));
	_setEnv("HTTP_COOKIE", _request->getHeader("Cookie"));
	_setEnv("HTTP_HOST", _request->getHeader("Host"));
	_setEnv("HTTP_REFERER", _request->getHeader("Referer"));
	_setEnv("HTTP_USER_AGENT", _request->getHeader("User-Agent"));

	_setEnv("SCRIPT_NAME", _request->getTarget());
	if (_is_post == false)
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
		char *		tmp = new char[str.size() + 1];
		std::strcpy(tmp, str.c_str());
		newEnv[i] = tmp;
	}
	newEnv[i] = NULL;

	return newEnv;
}

void Cgi::process(void) {
	_processed = true;
	_start_time = std::time(NULL);

	_forkPid = fork();
	if (_forkPid < 0)
		throw std::runtime_error("fork failed D:");
	else if (_forkPid == 0) {
		if (_is_post == true) {
			dup2(_stdin_pipe[PIPE_READ], STDIN_FILENO);
			close(_stdin_pipe[PIPE_READ]);
			close(_stdin_pipe[PIPE_WRITE]);
		}

		dup2(_stdout_pipe[PIPE_WRITE], STDOUT_FILENO);
		close(_stdout_pipe[PIPE_READ]);
		close(_stdout_pipe[PIPE_WRITE]);

		char * argv[] = {const_cast<char *>(_script_path.c_str()), NULL};
		char **env = _genEnv();

		if (execve(_script_path.c_str(), argv, env) == -1) {
			std::stringstream str;
			str << "execve failed D:";
			str << errno;
			_log->error(str.str());
			for (int i = 0; env[i] != NULL; i++)
				delete env[i];
			delete env;
			exit(EXIT_FAILURE);
		}
	} else {
		if (_is_post)
			close(_stdin_pipe[PIPE_READ]);
		close(_stdout_pipe[PIPE_WRITE]);
		// waitpid(_forkPid, NULL, 0);
	}
}

std::string Cgi::str(void) {
	int				   max = _conf->getMaxBody();
	char			   buffer[1024];
	std::ostringstream str;

	while (max) {
		ssize_t count = read(_stdout_pipe[0], buffer, sizeof(buffer));
		if (count > 0) {
			str.write(buffer, count);
			max -= count;
		} else if (count == 0) {
			break;
		} else
			break;
	}
	return str.str();
}
