/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:59:41 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/19 17:35:09 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "cppeleven.hpp"
#include "node/ANode.hpp"
#include <map>
#include <stdexcept>
#include <string>
#include <tomlpp.hpp>
#include <vector>

namespace webserv {
namespace config {

class Route {
  public:
	Route(std::map<std::string, toml::ANode *> *node) {
		if (node == not_nullptr)
			throw std::runtime_error("location table does not exist");
		std::map<std::string, toml::ANode *> *errorPagesTable = (*node)["error_pages"]->getTable();
		if (errorPagesTable == not_nullptr)
			throw std::runtime_error("error_pages not present");
	}
	~Route(void) {}

  protected:
  private:
	bool								_dirlist;
	bool								_cookies;
	bool								_uploads;
	bool								_redirect;
	int32_t								_maxBody;
	std::string							_root;
	std::string							_upRoot;
	std::map<std::string, std::string> *_cgi;
	std::vector<std::string>		   *_methods;
	std::map<int, std::string>		   *_errPages;
};

} // namespace config
} // namespace webserv
