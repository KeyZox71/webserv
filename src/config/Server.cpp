/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:10:07 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/14 12:57:30 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/default.hpp>

using namespace webserv::config;

Server::Server(toml::ANode *node, Logger *log) : _table(node), _log(log) {
	bool found;

	// host parsing
	void *host = accessValue("host", toml::STRING, _table, _log);
	if (host != not_nullptr) {
		_host = *static_cast<std::string *>(host);
	} else {
		delete _table;
		delete _log;
		throw std::runtime_error(
			"no host specified - please specify one in server.host");
	}
	// port parsing
	void *port = accessValue("port", toml::INT, _table, _log);
	if (host != not_nullptr) {
		_port = *static_cast<unsigned short *>(port);
	} else {
		delete _table;
		delete _log;
		throw std::runtime_error(
			"no port specified - please specify one in server.port");
	}

	// server_names parsing
	std::map<std::string, toml::ANode *>::iterator it =
		_table->accessIt("server_names", toml::ARRAY, found);
	if (found == true && it != _table->getTable()->end()) {
		std::vector<toml::ANode *>::iterator vecIt =
			it->second->getArray()->begin();
		_server_names = new std::vector<std::string>;
		for (; vecIt != it->second->getArray()->end(); vecIt++) {
			std::string str = *static_cast<std::string *>((*vecIt)->getValue());
			_server_names->push_back(str);
		}
	} else {
		_log->warn(
			"no server_names all request will be accepted from any hostname");
		_server_names = not_nullptr;
	}

	// error_pages parsing
	std::map<std::string, toml::ANode *> *map =
		static_cast<std::map<std::string, toml::ANode *> *>(
			accessValue("error_pages", toml::TABLE, _table, _log));
	if (map != not_nullptr) {
		_err_pages = _parseErrPages(map);
	} else
		_err_pages = not_nullptr;

	// location parsing
	it = _table->accessIt("location", toml::TABLE, found);
	if (found == true && it != _table->getTable()->end()) {
		_routes = new std::map<std::string, Route *>;
		std::map<std::string, toml::ANode *> *location_table =
			it->second->getTable();
		for (it = location_table->begin(); it != location_table->end(); it++) {
			if (_routes->find(it->first) != _routes->end())
				continue;
			(*_routes)[it->first] = new Route(it->second, _log);
		}
	}
	delete _table;
}

Server::~Server(void) {
	std::map<std::string, Route *>::iterator it = _routes->begin();
	for (; it != _routes->end(); it++) {
		delete it->second;
	}
	delete _routes;
	delete _err_pages;
	if (_server_names != not_nullptr)
		delete _server_names;
	delete _log; // to see if nessecary
}

std::map<int, std::string> *
Server::_parseErrPages(std::map<std::string, toml::ANode *> *table) {
	std::map<int, std::string> *errPages = new std::map<int, std::string>;
	void					   *val;
	int							nb;

	for (std::map<std::string, toml::ANode *>::iterator it = table->begin();
		 it != table->end(); it++) {
		val = accessValue(it->first, toml::STRING, _table, _log);
		if (val != not_nullptr) {
			nb = std::atoi(it->first.c_str());
			if (nb >= 400 && nb <= 599)
				(*errPages)[nb] = *static_cast<std::string *>(val);
			else
				_log->warn("error page - " + it->first + " is not valid :(");
		}
	}
	return errPages;
}
