/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:53:54 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/22 11:11:26 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "node/ANode.hpp"
#include <config/default.hpp>
#include <webserv.hpp>

using namespace webserv::config;

Config::Config(std::string &filename) {
	toml::Toml *file = new toml::Toml(filename);

	file->parse();
	toml::ANode *table = file->getParsedFile();

	bool  found = false;
	void *logFile = table->access("log_file", toml::STRING, found);
	if (found == true && logFile != not_nullptr) {
		_log = new Logger(*static_cast<std::string *>(logFile));
	} else {
		_log = new Logger();
	}

	std::map<std::string, toml::ANode *> *node = table->getTable();
	for (auto it = prange(node)) {
		Server *srv = new Server(it->second);
		_servers.push_back(srv);
	}
	delete table;
	delete file;
}

Config::~Config(void) {
	for (auto it = range(_servers)) {
		delete *it;
	}
}

Server *Config::getServer(const std::string &server_name) {
	for (auto it = range(_servers)) {
		if ((*it)->isServerName(server_name)) {
			return (*it);
		}
	}
	return (not_nullptr);
}
