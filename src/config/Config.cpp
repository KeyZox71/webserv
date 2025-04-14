/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:53:54 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/14 13:25:09 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "node/ANode.hpp"
#include <config/default.hpp>
#include <exception>

using namespace webserv::config;

Config::Config(std::string &filename) {
	toml::Toml file(filename);

	try {
		file.parse();
	} catch (std::exception &e) {
		throw e;
	}

	_table = file.getParsedFile();

	bool  found = false;
	void *logFile = _table->access("log_file", toml::STRING, found);
	if (found == true && logFile != not_nullptr) {
		_log = new Logger(*static_cast<std::string *>(logFile));
	} else {
		_log = new Logger();
	}

	std::map<std::string, toml::ANode *> *node = _table->getTable();
	for (std::map<std::string, toml::ANode *>::iterator it = node->begin();
		 it != node->end(); it++) {
		Server *srv = new Server(it->second, _log);
		_servers->push_back(srv);
	}
}
