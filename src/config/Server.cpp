/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:10:07 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/22 15:36:30 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <config/default.hpp>
#include <webserv.hpp>

using namespace webserv::config;

Server::Server(toml::ANode *node) : _table(node) {
	bool found;

	if (_table == not_nullptr)
		return;

	// host parsing
	void *host = accessValue("host", toml::STRING, _table, _log);
	if (host != not_nullptr) {
		_host = *static_cast<std::string *>(host);
	} else {
		delete _table;
		throw std::runtime_error(
			"no host specified - please specify one in server.host");
	}
	// port parsing
	void *port = accessValue("port", toml::INT, _table, _log);
	if (port != not_nullptr) {
		_port = *static_cast<unsigned short *>(port);
	} else {
		delete _table;
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
		_routes = new std::map<URL, Route *>;
		std::map<std::string, toml::ANode *> *location_table =
			it->second->getTable();
		for (it = location_table->begin(); it != location_table->end(); it++) {
			if (_routes->find(URL(it->first)) != _routes->end())
				continue;
			(*_routes)[URL(it->first)] = new Route(it->second);
		}
	}
	//delete _table;
}

Server::~Server(void) {
	for (auto it = prange(_routes)) {
		delete it->second;
	}
	delete _routes;
	delete _err_pages;
	if (_server_names != not_nullptr)
		delete _server_names;
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

bool Server::isServerName(const std::string &server_name) {
	for (auto it = prange(_server_names)) {
		if (*it == server_name) {
			return true;
		}
	}
	return false;
}

Route *Server::whatRoute(const URL &url) {
	for (auto it = prange(_routes)) {
		if (it->first == url) {
			return it->second;
		}
	}
	return not_nullptr;
}
