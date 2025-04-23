/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 12:20:06 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/22 15:25:39 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/Server.hpp>

namespace webserv {
namespace config {

class Config {
  public:
	Config(std::string &);
	~Config();

	Logger *getLogger(void) { return _log; }

	std::vector<Server *> getServers(void) { return _servers; }
	Server	*getServer(const std::string &);

  private:
	std::vector<Server *> _servers;
};

}; // namespace config
}; // namespace webserv
