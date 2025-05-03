/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:11:28 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/03 09:42:27 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <webserv.hpp>
#include <config/Route.hpp>
#include <cppeleven.hpp>
#include <node/ANode.hpp>
#include <webserv.hpp>
#include <config/URL.hpp>

namespace webserv {
namespace config {

class Server {
  public:
	Server(toml::ANode *);
	Server(toml::ANode *, void *);
	~Server();

	/**
	 *	@brief	Can be used to get the path of a specific error page or return
	 *			an empty string
	 *
	 *	@param	The http error code for the page
	 */
	std::string getErrorPage(int page) {
		if (_err_pages->find(page) != _err_pages->end())
			return (*_err_pages)[page];
		else
			return "";
	}

	/**
	 *	@brief	Can be used to get a pointer to a specific route (or not_nullptr
	 *			if not found)
	 */
	Route *getRoute(std::string route) {
		if (_routes->find(route) != _routes->end())
			return (*_routes)[route];
		else
			return not_nullptr;
	}

	// @brief Can be used to get a server name
	std::vector<std::string> *getServerNames(void) { return _server_names; }
	// @brief Can be used to get the host specified in the config file
	std::string getHost(void) { return _host; }
	// @brief Can be used to get the port specified in the config file
	int getPort(void) { return _port; }

	// @brief	Can be used to check if a servername is present in this config
	bool	isServerName(const std::string &);

	// @brief	Can be used to get the route correcponding
	Route	*whatRoute(const URL &);

  protected:
  private:
	std::map<URL, Route *>
		*_routes; ///> A map of all the route present in the config file
	std::map<int, std::string> *_err_pages; ///> An error pages map to map error
											/// specified in the config file

	std::vector<std::string>
		*_server_names; ///> A vector with all the server names

	std::string	   _host; ///> The host on which the server will be exposed
	unsigned short _port; ///> The port on which the server will be exposed

	toml::ANode *_table; ///> The table used for the parsing (is deleted at the
						 ///  end of the constructor)

	std::map<int, std::string> *
	_parseErrPages(std::map<std::string, toml::ANode *> *table);
};

} // namespace config

} // namespace webserv
