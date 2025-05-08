/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/06 19:36:17 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/default.hpp>

using namespace webserv;

Cgi::Cgi(http::ARequest *req, config::Server *conf) : _conf(conf), _request(req) {
	_initEnvp();
}

void	Cgi::_initEnvp(void)
{

	setEnv("REQUEST_METHOD", _request->getMethod()); //url get ou pour post
	setEnv("?", _request->getParamRequest()); //apres le ?
	setEnv("TYPE", _request->getType()); //type de requete, genre html
	setEnv("LENGTH", _request->getLength()); //taille pour les requetes post
	setEnv("SCRIPT", _request->getScript()); //chemin relatif du serveur
	setEnv("MORE_INFO", _request->getMoreInfo()); //chemin supplementaire
}

char *Cgi::getEnv(std::string &key) {
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
	if (forkPid == 0)
	{
		int fd = open("kanel/cheminfichier",  O_RDONLY); //chemin vers ce que je dois ouvrir
		if (fd == -1)
			//throw erreur
		dup2(fd, 0);
		dup2(pipefd[1], 1);
		close(fd);
		close(pipefd[0]);
		close(pipefd[1]);

		std::string cgipath = "kanel/chemincgi"; // chemin du cgi
		char *envp[] = {getEnv("REQUEST_METHOD"), getEnv("?"), getEnv("TYPE"), getEnv("LENGTH"), NULL };
		execve(cgipath, NULL, env);
		//throw si execve echou
	}
	else if (forkPid >= 1)
	{
		close(pipefd[1]);

		int nb;
		waitpid(forkPid, &nb, 0);

		char buffer[4096] //jsp quoi mettre le temps
		while (read(pipefd[0], buffer, sizeof(buffer)))
		{
			//tranfert donnees
		}
		close(pipefd[0]);
	}
	else
		throw error;
}
