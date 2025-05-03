/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:20:06 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/03 09:42:35 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "cppeleven.hpp"
#include <config/Server.hpp>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

namespace webserv {
namespace config {

class Config {
  public:
	Config(std::string &);
	~Config();

	Logger *getLogger(void) { return _log; }

	std::vector<Server *> getServers(void) { return _servers; }
	Server				 *getServer(size_t i) {
		  try {
			  Server *srv = _servers.at(i);
			  return srv;
		  } catch (std::out_of_range &e) {
			  return not_nullptr;
		  }
	}

  private:
	std::vector<Server *> _servers;
};

}; // namespace config
}; // namespace webserv
