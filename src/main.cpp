/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/06 19:20:55 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <config/Config.hpp>
#include <config/default.hpp>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <help.hpp>
#include <log.hpp>
#include <requests/default.hpp>
#include <server/default.hpp>
#include <sstream>
#include <tomlpp.hpp>
#include <unistd.h>
#include <webserv.hpp>

namespace webserv {
Logger *_log = not_nullptr;
}

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
	if (access(av[1], F_OK) < 0) {
		std::stringstream str;
		str << "File : " << av[1] << " could not be opened";
		log.error(str.str());
		return EXIT_FAILURE;
	}

	_log = not_nullptr;
	config::Config *conf;
	try {
		std::string str = av[1];
		conf = new config::Config(str);
	} catch (std::exception &) {
		if (_log != not_nullptr)
			delete _log;
		return 1;
	}
	if (signal(SIGINT, &ft_sig) == SIG_ERR) {
		conf->getLogger()->error("could not bind sigint :(");
		return EXIT_FAILURE;
	}

	webserv::server::Server *serv = new webserv::server::Server(conf);

	delete serv;
	delete _log;
	delete conf;
}
