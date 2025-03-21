/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:59:41 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/21 11:09:38 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "cppeleven.hpp"
#include "log.hpp"
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

	Logger *_log;

	bool						_methods[3]; // 1: GET, 2: POST, 3: DELETE
	std::map<int, std::string> *_err_pages;
};

} // namespace config
} // namespace webserv
