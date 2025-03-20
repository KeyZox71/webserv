/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:11:28 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/19 23:09:56 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Route.hpp"

namespace webserv {
namespace config {

class Server {
  public:
  protected:
  private:
	Route					*_default;
	std::vector<Route *>	*_routes;
	std::string				 host;
	std::vector<std::string> server_names;
};

} // namespace config

} // namespace webserv
