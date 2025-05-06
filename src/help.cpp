/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:08:36 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/06 19:19:43 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <log.hpp>
#include <fstream>
#include <help.hpp>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <webserv.hpp>

using namespace webserv;

void _printHelp(void) {
	std::cout << "-------------- WEBSERV --------------" << std::endl;
	std::cout << "- --help : Print this message" << std::endl;
	std::cout << "- --generate : Generate a sample config" << std::endl;
	std::cout << "- --version : Gives you the version of the server"
			  << std::endl;
	std::cout << "-------------------------------------" << std::endl;
}

void _generateConf(void) {
	webserv::Logger _log;
	if (access(SAMPLE_CONF_PATH, F_OK) == 0) {
		_log.warn(std::string(SAMPLE_CONF_PATH) + " already exist, aborting");
	} else {
		std::stringstream str;
		str << "generating config into " << SAMPLE_CONF_PATH << "...";
		_log.info(str.str());
		std::ofstream file(SAMPLE_CONF_PATH);
		if (file.is_open()) {
			file << "[server]\nhost = \"0.0.0.0\"\nport = "
				"80\n\n[server.location./]\nmethods = { \"GET\" }\nroot "
				"= \"/var/www/html\"\ndirlist = true\nclient_max_body_size "
				"= \"10M\"\n";
			file.close();
			_log.info("config file successfully generated");
		} else {
			_log.warn("could not generate, sample config file");
		}
	}
}

void _printVersion(void) {
	Logger log;
	std::stringstream str;
	str << "you are running webserv " << WEBSRV_VERSION;
	log.info(str.str());
}

bool help(int ac, char **av) {
	if (ac < 2) {
		_printHelp();
		return true;
	}
	std::string option = av[1];
	if (option == "--help" || option == "-v") {
		_printHelp();
		return true;
	}
	else if (option == "--generate" || option == "-g") {
		_generateConf();
		return true;
	}
	else if (option == "--version" || option == "-v") {
		_printVersion();
		return true;
	}
	else
		return false;
}
