/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:45:43 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 19:32:38 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/default.hpp>
#include <fcntl.h>
#include <log.hpp>
#include <netinet/in.h>
#include <poll.h>
#include <server/PfdManager.hpp>
#include <stdexcept>
#include <vector>
#include <webserv.hpp>

namespace webserv {
namespace server {

class Client;

class Server {
  public:
	Server(void);
	~Server(void);

  protected:
  private:
	/**
	 *	@brief	Used to setup the webserver (primarly socket)
	 */
	void _setup(void);
	/**
	 *	@brief	Used to run the webserver
	 */
	void _run(void);

	/**
	 *	@brief	Can be used to fill the vector passed as parameters with all the
	 *			port and host in the config
	 *	@param	The vector of host
	 *	@param	The vector of port
	 */
	int _fillHostsPorts(std::vector<std::string> &, std::vector<int> &);

	/**
	 *	@brief	Can be used to open a socket with a specific port and host
	 *	@param	The host
	 *	@param	The port
	 */
	int _createSocket(std::string, int);

	/**
	 *	@brief	Can be used to check if an fd is one of the socket or not
	 *
	 *	@param	the fd to check
	 */
	bool _isServerSocket(int fd) {
		for (std::vector<int>::iterator it = _fds_server.begin();
			 it != _fds_server.end(); it++) {
			if (fd == *it) {
				return true;
			}
		}
		return false;
	}

	Client *_getClient(int);
	std::vector<int> _fds_server; // The fds of the sockets
	// std::vector<struct pollfd> _client_fds; // A vector of all the poll fd
	std::vector<Client *> _client_data; // vector of all the client sockaddr_in

	void _handle_srv(size_t i);
	void _handle_client(size_t *i);
	void _handle_resource(size_t i);

};

}; // namespace server
}; // namespace webserv
