/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:20:06 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/18 10:03:09 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config/default.hpp"

namespace webserv {
namespace config {

class Config {
  public:
	Config(std::string &);
	~Config();

	Logger *getLogger(void) { return _log; }

	std::vector<Server *> *getServers(void) { return _servers; }

  private:
	std::vector<Server *> *_servers;
};

}; // namespace config
}; // namespace webserv
