/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:37:02 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/26 08:19:25 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "log.hpp"
#include "node/default.hpp"
#include <config/default.hpp>
#include <map>
#include <string>

using namespace webserv::config;

std::map<std::string, std::string> *Route::_parseCGI(toml::ANode *table) {
	std::map<std::string, std::string> *cgi =
		new std::map<std::string, std::string>;
	void *val;

	for (std::map<std::string, toml::ANode *>::iterator it =
			 table->getTable()->begin();
		 it != table->getTable()->end(); it++) {
		val = accessValue(it->first, toml::STRING, table, _log);
		if (val != not_nullptr) {
			if (cgi->find(it->first) != cgi->end())
				continue;
			else
				(*cgi)[it->first] = *static_cast<std::string *>(val);
		}
	}

	return cgi;
}

void Route::_parseMethods(std::vector<toml::ANode *> *table) {
	std::string val;

	_methods[0] = false;
	_methods[1] = false;
	_methods[2] = false;
	for (std::vector<toml::ANode *>::iterator it = table->begin();
		 it != table->end(); it++) {
		if ((*it)->type() == toml::STRING) {
			val = *static_cast<std::string *>((*it)->getValue());
			if (val == "GET")
				_methods[0] = true;
			if (val == "POST")
				_methods[1] = true;
			if (val == "DELETE")
				_methods[2] = true;
		}
	}
}

Route::Route(toml::ANode *table, Logger *logger)
	: _max_body(10485760), _log(logger) {
	void *val;
	bool  found;

	_log = logger;
	_table = table;
	if (_table->type() != toml::TABLE) {
		_log->warn("location need to be a table and not a :" +
				   toml::nodeTypeToStr(_table->type()));
		return;
	}
	val = accessValue("redirect", toml::STRING, _table, _log);
	if (val != not_nullptr) {
		_root = *static_cast<std::string *>(val);
		_redirect = true;
		return;
	} else
		_redirect = false;
	val = accessValue("dirlist", toml::BOOL, _table, _log);
	if (val != not_nullptr)
		_dirlist = *static_cast<bool *>(val);
	else
		_dirlist = true;
	val = accessValue("cookies", toml::BOOL, _table, _log);
	if (val != not_nullptr)
		_cookies = *static_cast<bool *>(val);
	else
		_cookies = false;
	val = accessValue("upload_path", toml::STRING, _table, _log);
	if (val != not_nullptr)
		_up_root = *static_cast<std::string *>(val);
	else
		_up_root = "";
	val = accessValue("index", toml::STRING, _table, _log);
	if (val != not_nullptr)
		_index = *static_cast<std::string *>(val);
	else
		_index = "index.html";
	val = accessValue("root", toml::STRING, _table, _log);
	if (val != not_nullptr)
		_root = *static_cast<std::string *>(val);
	else
#ifdef PKGS
		_root = "/var/www/html"
#else
		_root = "./html";
#endif
			val =
				accessValue("client_max_body_size", toml::STRING, _table, _log);
	if (val != not_nullptr)
		_max_body = _parseSize(*static_cast<std::string *>(val));
	std::map<std::string, toml::ANode *>::iterator it =
		_table->accessIt("cgi", toml::TABLE, found);
	if (found == true && it != _table->getTable()->end())
		_cgi = _parseCGI(it->second);
	else
		_cgi = not_nullptr;
	val = accessValue("methods", toml::ARRAY, _table, _log);
	if (val != not_nullptr)
		_parseMethods(static_cast<std::vector<toml::ANode *> *>(val));
	else {
		_methods[0] = true;
		_methods[1] = false;
		_methods[2] = false;
	}
}

Route::~Route(void) {
	if (_redirect == false)
		if (_cgi != not_nullptr)
			delete _cgi;
}
