/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:53:54 by adjoly            #+#    #+#             */
/*   Updated: 2025/07/12 21:40:21 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "log.hpp"
#include "node/ANode.hpp"
#include "node/default.hpp"
#include "webserv.hpp"
#include <config/default.hpp>
#include <exception>
#include <netinet/in.h>
#include <stdexcept>

using namespace webserv::config;

Config::Config(std::string &filename) {
	toml::Toml *file = new toml::Toml(filename);

	try {
		file->parse();
	} catch (const std::exception &e) {
		// if (file->getParsedFile() != not_nullptr)
			// delete file->getParsedFile();
		std::runtime_error excep(e.what());
		delete file;
		throw excep;
	}
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
		if (it->second->type() == toml::TABLE) {
			_log->info("taking server from table : " + it->first);
			try {
				Server *srv = new Server(it->second);
				_servers.push_back(srv);
			} catch (std::runtime_error &e) {
				_log->error(e.what());
				if (!_servers.empty()) {
					for (auto it = range(_servers))
						delete (*it);
				}
				delete table;
				delete file;
				throw e;
			}
		}
	}
	delete table;
	delete file;
}

Config::~Config(void) {
	for (auto it = range(_servers)) {
		delete *it;
	}
}
