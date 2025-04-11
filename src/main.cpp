/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/11 17:51:33 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Server.hpp>
#include <cstdlib>
#include <exception>
#include <help.hpp>
#include <requests/default.hpp>
#include <tomlpp.hpp>
#include <unistd.h>
#include <webserv.hpp>

int main(int ac, char **av) {
	if (help(ac, av)) {
		return EXIT_SUCCESS;
	}
	std::cout << "Starting server..." << std::endl;
	if (access(av[1], F_OK) < 0) {
		std::cout << "File : " << av[1] << " could not be opened" << std::endl;
		return EXIT_FAILURE;
	}

	config::Server *conf;
	try {
		conf = new config::Server(av[1]);
	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	delete conf;
}
