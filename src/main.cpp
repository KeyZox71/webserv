/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/07/15 20:28:04 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <config/default.hpp>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <help.hpp>
#include <log.hpp>
#include <requests/default.hpp>
#include <server/ResourceManager.hpp>
#include <server/default.hpp>
#include <sstream>
#include <tomlpp.hpp>
#include <unistd.h>
#include <webserv.hpp>

namespace webserv {
Logger *							   _log = not_nullptr;
std::vector<server::AClientResource *> server::ResourceManager::_res;
config::Config *					   config::_conf = not_nullptr;
server::Server						   *_server_wtf;
} // namespace webserv

int _sig = 0;

void ft_sig(int sig) {
	_sig = sig;
	std::stringstream str;
	str << "sig hitted = ";
	str << sig;
	_log->info(str.str());
}

int main(int ac, char **av) {
	Logger log;
	if (help(ac, av)) {
		return EXIT_SUCCESS;
	}
	log.info("Starting server...");
	if (ac < 2) {
		if (access(SAMPLE_CONF_PATH, F_OK) < 0) {
			std::stringstream str;
			str << "File : " << SAMPLE_CONF_PATH << " could not be opened";
			log.error(str.str());
			return EXIT_FAILURE;
		}
	} else if (access(av[1], F_OK) < 0) {
		std::stringstream str;
		str << "File : " << av[1] << " could not be opened";
		log.error(str.str());
		return EXIT_FAILURE;
	}

	_log = not_nullptr;
	try {
		std::string str;
		if (ac < 2) {
			str = SAMPLE_CONF_PATH;
		} else {
			str = av[1];
		}
		config::_conf = new config::Config(str);
	} catch (const std::exception &e) {
		Logger log;
		log.error(e.what());
		if (_log != not_nullptr)
			delete _log;
		return 1;
	}
	if (signal(SIGINT, &ft_sig) == SIG_ERR) {
		config::_conf->getLogger()->error("could not bind sigint :(");
		return EXIT_FAILURE;
	}

	_server_wtf = new webserv::server::Server();

	delete _server_wtf;
	delete _log;
	delete config::_conf;
}
