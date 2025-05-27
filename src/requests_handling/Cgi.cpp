/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/27 22:26:52 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// WARN: construtor will probably be changed and practicly do nothing
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
}

Cgi::~Cgi(void) { log("➖", "Cgi", "destructor called"); }

void Cgi::_prep(void) {
	if (_is_post)
		if (pipe(_stdin_pipe) == -1)
			throw std::runtime_error("stdin pipe failed for cgi D:");
	if (pipe(_stdout_pipe) == -1)
		throw std::runtime_error("stdout pipe failed for cgi D:");
	_script_path = _conf->getRootDir() + _request->getTarget();
	_fd = _stdout_pipe[STDIN_FILENO];
	_pfd_event = POLLIN;
	if (access(_script_path.c_str(), X_OK))
		throw std::runtime_error(
			"script is not executable please run : chmod +x " + _script_path);
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
		char *		tmp = new char[str.size() + 1];
		std::strcpy(tmp, str.c_str());
		newEnv[i] = tmp;
	}
	newEnv[i] = NULL;

	return newEnv;
}

void Cgi::process(void) {
	_processed = true;
	pid_t forkPid;

	forkPid = fork();
	if (forkPid < 0)
		throw std::runtime_error("fork failed D:");
	else if (forkPid == 0) {
		if (_is_post) {
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
			str << "how did you do that ???? : ";
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
		waitpid(forkPid, NULL, 0);
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
		}
		else
			break;
	}
	if (_is_post)
		ResourceManager::remove(_stdin_pipe[PIPE_WRITE]);
	return str.str();
}
