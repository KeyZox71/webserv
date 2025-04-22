/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/22 14:50:32 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/default.hpp>
#include <csignal>
#include <server/default.hpp>
#include <config/Config.hpp>
#include <cstdlib>
#include <exception>
#include <help.hpp>
#include <requests/default.hpp>
#include <tomlpp.hpp>
#include <unistd.h>
#include <log.hpp>
#include <webserv.hpp>

namespace webserv {
    Logger *_log = not_nullptr;
}

int	_sig = 0;

void	ft_sig(int sig) {
	_sig = sig;
}

int main(int ac, char **av) {
	if (help(ac, av)) {
		return EXIT_SUCCESS;
	}
	std::cout << "Starting server..." << std::endl;
	if (access(av[1], F_OK) < 0) {
		std::cout << "File : " << av[1] << " could not be opened" << std::endl;
		return EXIT_FAILURE;
	}


	config::Config *conf;
	try {
		std::string str = av[1];
		conf = new config::Config(str);
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	if (signal(SIGINT, &ft_sig) == SIG_ERR) {
		conf->getLogger()->error("could not bind sigint :(");
		return EXIT_FAILURE;
	}

	webserv::Server *serv = new webserv::Server(conf);

	delete serv;
	delete conf;
}
