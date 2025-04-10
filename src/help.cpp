/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:08:36 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/10 14:19:20 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.hpp"
#include <fstream>
#include <help.hpp>
#include <sys/stat.h>
#include <unistd.h>
#include <webserv.hpp>

void _printHelp(void) {
	std::cout << "-------------- WEBSERV --------------" << std::endl;
	std::cout << "- --help : Print this message" << std::endl;
	std::cout << "- --generate : Generate a sample config" << std::endl;
	std::cout << "- --version : Gives you the version of the server"
			  << std::endl;
	std::cout << "-------------------------------------" << std::endl;
}

void _generateConf(void) {
	webserv::Logger _log("");
	if (access("./sample.conf", F_OK) == 0) {
		_log.warn(std::string(SAMPLE_CONF_PATH) + " already exist, aborting");
	} else {
		_log.info("generating config into ./sample.conf...");
		std::ofstream file("./sample.conf");
		if (file.is_open()) {
			file << "[server]\nhost = \"localhost\"\nport = "
				"8080\n\n[server.location./]\nmethods = { \"GET\" }\nroot "
				"= \"/var/www/html\"\ndirlist = true\nclient_max_body_size "
				"= \"10M\"";
			file.close();
			_log.info("config file successfully generated");
		} else {
			_log.warn("could not generate, sample config file");
		}
	}
}

void _printVersion(void) {
	std::cout << "You are running : Webserv " << WEBSRV_VERSION << std::endl;
}

void help(int ac, char **av) {
	if (ac < 2) {
		_printHelp();
		return;
	}
	std::string option = av[1];
	if (option == "--help")
		_printHelp();
	else if (option == "--generate" || option == "-g")
		_generateConf();
	else if (option == "--version" || option == "-v")
		_printVersion();
}
