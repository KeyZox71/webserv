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
#include "node/ANode.hpp"
#include "node/default.hpp"
#include <config/default.hpp>
#include <log.hpp>
#include <map>
#include <sstream>
#include <string>

using namespace webserv::config;

std::vector<std::string> *Route::_parseCGI(toml::ANode *table) {
	std::vector<std::string> *cgi = new std::vector<std::string>;

	for (auto it = prange(table->getArray())) {
		if ((*it)->type() == toml::STRING) {
			std::string str = *static_cast<std::string *>((*it)->getValue());
			cgi->push_back(str);
		} else {
			std::stringstream str;
			str << "Was expecting a: " << toml::nodeTypeToStr(toml::STRING);
			str << ", but got a: " << toml::nodeTypeToStr((*it)->type());
			_log->warn(str.str());
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

Route::Route(toml::ANode *table) : _max_body(10485760) {
	void *val;
	bool  found;

	if (table == not_nullptr) {
		_log->warn("is empty");
		_index = "index.html";
		_redirect = false;
		_dirlist = false;
		_up_root = "";
#ifdef PKGS
		_root = "/usr/share/webserv";
#else
		_root = "./html";
#endif
		_cgi = not_nullptr;
		return;
	}
	_table = table;
	if (_table->type() != toml::TABLE) {
		_log->warn("location need to be a table and not a :" +
				   toml::nodeTypeToStr(_table->type()));
		_index = "index.html";
		_redirect = false;
		_dirlist = false;
		_up_root = "";
#ifdef PKGS
		_root = "/usr/share/webserv";
#else
		_root = "./html";
#endif
		_cgi = not_nullptr;
		_methods[0] = true;
		_methods[1] = false;
		_methods[2] = false;
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
		_dirlist = false;
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
		_root = "/usr/share/webserv";
#else
		_root = "./html";
#endif
	val = accessValue("client_max_body_size", toml::STRING, _table, _log);
	if (val != not_nullptr)
		_max_body = _parseSize(*static_cast<std::string *>(val));
	std::map<std::string, toml::ANode *>::iterator it =
		_table->accessIt("cgi", toml::ARRAY, found);
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
