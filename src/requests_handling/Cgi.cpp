/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/30 09:49:32 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requests/default.hpp"

using namespace webserv;

Cgi::Cgi(http::ARequest *req, config::Server *conf) : _conf(conf), _request(req) {
	_initEnvp();
}

void	Cgi::_initEnvp(void) {
	//_envp[] = "";
}

std::string Cgi::getEnv(std::string &key) {
	auto it = _envp.find(key);
	if (it != _envp.end()) {
		return it->second;
	}
	return "";
}

void	Cgi::setEnv(std::string &key, std::string value) {
	_envp[key] = value;
}

void	Cgi::process() {
	int	pipefd[2];
	int	forkPid;

	if (pipe(pipefd) <= -1) {
		//throw error
	}

	forkPid = fork();
	if (forkPid == 0) {
		// in fork
	}
}
