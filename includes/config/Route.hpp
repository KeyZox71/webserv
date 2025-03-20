/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:59:41 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/20 09:27:58 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "cppeleven.hpp"
#include "node/ANode.hpp"
#include "node/Array.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <map>
#include <stdexcept>
#include <string>
#include <tomlpp.hpp>
#include <vector>

namespace webserv {
namespace config {

int32_t parseSize(std::string size) {
	if (size[size.size()] == 'M')
		return std::atoi(size.c_str()) * 1024 * 1024;
	if (size[size.size()] == 'K')
		return std::atoi(size.c_str()) * 1024;
	if (isalpha(size[size.size()]))
		return std::atoi(size.c_str());
	return -1;
}

class Route {
  public:
	Route(std::map<std::string, toml::ANode *> *node) {
		if (node == not_nullptr)
			throw std::runtime_error("location table does not exist");

		_methods[0] = false;
		_methods[1] = false;
		_methods[2] = false;

		std::map<std::string, toml::ANode *>::iterator it;

		it = (*node).find("redirect");
		if (it != node->end()) {
			toml::ANode *redirNode = it->second;
			if (redirNode->type() == toml::STRING) {
				_redirect = true;
				_root = *(std::string *)redirNode->getValue();
				_cookies = false;
				_uploads = false;
				_dirlist = false;
				_cgi = not_nullptr;
				_methods[0] = false;
				_methods[1] = false;
				_methods[2] = false;
				_err_pages = not_nullptr;
				_upRoot = not_nullptr;
				return;
			}
		}

		it = (*node).find("uploads");
		if (it != node->end()) {
			toml::ANode *uploadsNode = it->second;
			if (uploadsNode->type() == toml::BOOL)
				_uploads = *(bool *)uploadsNode->getValue();
		}
		it = (*node).find("dirlist");
		if (it != node->end()) {
			toml::ANode *dirlistNode = it->second;
			if (dirlistNode->type() == toml::BOOL)
				_dirlist = *(bool *)dirlistNode->getValue();
		}
		it = (*node).find("cookies");
		if (it != node->end()) {
			toml::ANode *cookiesNode = it->second;
			if (cookiesNode->type() == toml::BOOL)
				_cookies = *(bool *)cookiesNode->getValue();
		}
		it = (*node).find("root");
		if (it != node->end()) {
			toml::ANode *rootNode = it->second;
			if (rootNode->type() == toml::STRING)
				_root = *(std::string *)rootNode->getValue();
		}
		it = (*node).find("client_max_body_size");
		if (it != node->end()) {
			toml::ANode *rootNode = it->second;
			if (rootNode->type() == toml::STRING) {
				int32_t maxBody =
					parseSize(*(std::string *)rootNode->getValue());
				if (maxBody != 0 && maxBody != -1)
					_max_body = maxBody;
				else {
					std::cerr << "root size is not correctttt"
							  << std::endl; /// change that later TODO
					_max_body = 10485760;
				}
			}
		}

		it = (*node).find("methods");
		if (it != node->end()) {
			toml::ANode *methodsNode = it->second;
			if (methodsNode->type() == toml::ARRAY) {
				std::vector<toml::ANode *>::iterator methods =
					methodsNode->getArray()->begin();
				for (; methods != methodsNode->getArray()->end(); methods++) {
					if ((*methods)->type() == toml::STRING) {
						std::string method =
							*(std::string *)(*methods)->getValue();
						if (method == "GET")
							_methods[0] = true;
						else if (method == "POST")
							_methods[1] = true;
						else if (method == "DELETE")
							_methods[2] = true;
						else
							throw std::runtime_error("da fuk that not a valid "
													 "methods bro"); // replace
																	 // with
																	 // warning
																	 // log
					} else {
						throw std::runtime_error(
							"error why did you put something elle than a "
							"string"); // replace with log func
					}
				}
			}
		}

		it = (*node).find("error_pages");
		if (it != node->end()) {
			toml::ANode *errorPagesNode = it->second;
			// for loop on it and add to error pages and atoi the name of the
			// page(need to be a deep copy) TODO
		}

		it = (*node).find("cgi");
		if (it != node->end()) {
			toml::ANode *cgiNode = it->second;
			// for loop on it and add to cgi (need to be a deep copy) TODO
		}
	}
	~Route(void) {}

  protected:
  private:
	bool _dirlist;
	bool _cookies;
	bool _uploads;
	bool _redirect;

	int32_t _max_body;

	std::string							_root;
	std::string							_upRoot;
	std::map<std::string, std::string> *_cgi;

	bool						_methods[3]; // 1: GET, 2: POST, 3: DELETE
	std::map<int, std::string> *_err_pages;
};

} // namespace config
} // namespace webserv
