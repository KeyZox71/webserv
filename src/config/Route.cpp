/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:37:02 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/24 10:51:29 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "log.hpp"
#include "node/ANode.hpp"
#include "node/default.hpp"
#include <config/Route.hpp>
#include <map>
#include <string>

using namespace webserv::config;

std::map<std::string, std::string> *
Route::_parseCGI(std::map<std::string, toml::ANode *> *table) {
	std::map<std::string, std::string> *cgi =
		new std::map<std::string, std::string>;
	void *val;

	for (std::map<std::string, toml::ANode *>::iterator it = table->begin();
		 it != table->end(); it++) {
		val = accessValue(it->first, toml::STRING);
		if (val != not_nullptr)
			cgi->insert(it->first, *static_cast<std::string *>(val));
	}

	return cgi;
}

std::map<int, std::string> *
Route::_parseErrPages(std::map<std::string, toml::ANode *> *table) {
	std::map<int, std::string> *errPages = new std::map<int, std::string>;
	void					   *val;
	int							nb;

	for (std::map<std::string, toml::ANode *>::iterator it = table->begin();
		 it != table->end(); it++) {
		val = accessValue(it->first, toml::STRING);
		if (val != not_nullptr) {
			nb = std::atoi(it->first.c_str());
			if (nb != 0 && (nb >= 400 && nb <= 599))
				(*errPages)[nb] = *static_cast<std::string *>(val);
			else
				_log->warn("error page - " + it->first + " is not valid :(");
		}
	}
	return errPages;
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

void Route::_defaultErrPages(void) {
	_err_pages = new std::map<int, std::string>;

	(*_err_pages)[400] = _root + "/400.html";
	(*_err_pages)[403] = _root + "/403.html";
	(*_err_pages)[404] = _root + "/404.html";
}

Route::Route(toml::ANode *table, Logger *logger)
	: _max_body(10485760), _log(logger) {
	void *val;

	_table = table;
	_log = logger;
	val = accessValue("redirect", toml::STRING);
	if (val != not_nullptr) {
		_root = *static_cast<std::string *>(val);
		_redirect = true;
		return;
	}
	val = accessValue("dirlist", toml::BOOL);
	if (val != not_nullptr)
		_dirlist = *static_cast<bool *>(val);
	else
		_dirlist = true;
	val = accessValue("cookies", toml::BOOL);
	if (val != not_nullptr)
		_cookies = *static_cast<bool *>(val);
	else 
		_cookies = false;
	val = accessValue("uploads", toml::BOOL);
	if (val != not_nullptr)
		_uploads = *static_cast<bool *>(val);
	val = accessValue("root", toml::STRING);
	if (val != not_nullptr)
		_root = *static_cast<std::string *>(val);
	else
#ifdef PKGS
		_root = "/var/www/html"
#else
		_root = "./html";
#endif
			val = accessValue("upload_path", toml::STRING);
	if (val != not_nullptr)
		_upRoot = *static_cast<std::string *>(val);
	val = accessValue("client_max_body_size", toml::STRING);
	if (val != not_nullptr)
		_max_body = _parseSize(*static_cast<std::string *>(val));
	val = accessValue("cgi", toml::TABLE);
	if (val != not_nullptr)
		_cgi =
			_parseCGI(static_cast<std::map<std::string, toml::ANode *> *>(val));
	else
		_cgi = not_nullptr;
	val = accessValue("error_pages", toml::TABLE);
	if (val != not_nullptr)
		_err_pages = _parseErrPages(
			static_cast<std::map<std::string, toml::ANode *> *>(val));
	else
		_err_pages = _defaultErrPages();
	val = accessValue("methods", toml::ARRAY);
	if (val != not_nullptr)
		_parseMethods(static_cast<std::vector<toml::ANode *> *>(val));
	else {
		_methods[0] = true;
		_methods[1] = false;
		_methods[2] = false;
	}
}

void *Route::accessValue(std::string name, toml::nodeType type) {
	void *val;
	bool  found;

	val = _table->access(name, type, found);
	if (val != not_nullptr) {
		return val;
	} else {
		if (found == false) {
			return not_nullptr;
		} else {
			_log->warn("found - " + name + " but is not " +
					   toml::nodeTypeToStr(type) + ", skipping...");
			return not_nullptr;
		}
	}
}
