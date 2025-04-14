/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:53:54 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/14 13:32:56 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/default.hpp>

using namespace webserv::config;

Config::Config(std::string &filename) {
	toml::Toml *file = new toml::Toml(filename);

	file->parse();
	_table = file->getParsedFile();

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
	delete _table;
	delete file;
}

Config::~Config(void) {
	std::vector<Server *>::iterator it = _servers->begin();
	for (; it != _servers->end() ; it++) {
		delete *it;
	}
	delete _servers;
}
