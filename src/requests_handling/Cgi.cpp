/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/15 12:46:38 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requests/Cgi.hpp"
#include "help.hpp"
#include <string>

using namespace webserv;

Cgi::Cgi(http::ARequest *req, config::Route *conf)
	: _conf(conf), _request(req) {
	_initEnvp();
}

void Cgi::_initEnvp(void) {
	std::stringstream str;
	str << WEBSRV_NAME << "/" << WEBSRV_VERSION;
	setEnv("SERVER_SOFTWARE", str.str());
	setEnv("SERVER_NAME", _request->getHeader("Host"));
	setEnv("SERVER_PROTOCOL", _request->getProtocol());
	// setEnv("SERVER_PORT", _request->get); // TODO need to get the port by a
	// way i dont know yet

	setEnv("GATEWAY_INTERFACE", "CGI/1.1");

	// setEnv("PATH_TRANSLATED", ); // TODO wtf should i put here i dont fcking
	// know setEnv("PATH_INFO", ); // TODO wut make no sense

	str.clear();
	str << _request->getBody().length();
	setEnv("CONTENT_LENGH", str.str());
	setEnv("CONTENT_TYPE", _request->getHeader("Content-Type"));
	// setEnv("REMOTE_ADDR", _request->get) // TODO don't have it yet need to be
	// passed to the requset :sob:
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

void Cgi::process() {
	int pipefd[2];
	int forkPid;

	if (pipe(pipefd) <= -1) {
		// throw error
	}

	forkPid = fork();
	if (forkPid == 0) {
		int fd = open("kanel/cheminfichier",
					  O_RDONLY); // chemin vers ce que je dois ouvrir
		if (fd == -1)
			// throw erreur
			dup2(fd, 0);
		dup2(pipefd[1], 1);
		close(fd);
		close(pipefd[0]);
		close(pipefd[1]);

		std::string cgipath = "kanel/chemincgi"; // chemin du cgi
		char *envp[] = {getEnv("REQUEST_METHOD"), getEnv("?"), getEnv("TYPE"),
						getEnv("LENGTH"), NULL};
		execve(cgipath, NULL, env);
		// throw si execve echou
	} else if (forkPid >= 1) {
		close(pipefd[1]);

		int nb;
		waitpid(forkPid, &nb, 0);

		char buffer[4096] // jsp quoi mettre le temps
			while (read(pipefd[0], buffer, sizeof(buffer))) {
			// tranfert donnees
		}
		close(pipefd[0]);
	} else
		throw error;
}
