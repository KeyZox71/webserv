/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:45:43 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/22 11:51:27 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <log.hpp>
#include <config/default.hpp>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/poll.h>
#include <vector>

namespace webserv {

struct client_data {
	sockaddr_in sock_data;
	pollfd		poll_fd;
};

class Server {
  public:
	Server(config::Config *);
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
	 *	@brief	Used to handle client request
	 *
	 *	@param	The fd of the client
	 */
	bool _handle_client(struct pollfd &, sockaddr_in *);

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

	config::Config
		   *_conf; // Pointer to the configuration class (with all config in)
	Logger *_log;  // Pointer to the log class
	std::vector<int>		   _fds_server; // The fds of the sockets
	std::vector<struct pollfd> _client_fds; // A vector of all the poll fd
	std::vector<sockaddr_in *>
		_client_data; // vector of all the client sockaddr_in
};

}; // namespace webserv
