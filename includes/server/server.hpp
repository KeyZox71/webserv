/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 17:45:43 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/11 19:22:43 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "log.hpp"
#include <config/default.hpp>
#include <fcntl.h>
#include <stdexcept>
#include <sys/poll.h>
#include <vector>

namespace webserv {

class Server {
  public:
	Server(config::Server *);
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
	 *	@param	The number of the client fd
	 */
	void _handle_client(int);

	config::Server
		   *_conf; ///> Pointer to the configuration class (with all config in)
	Logger *_log;  ///> Pointer to the log class
	int		_fd_server; ///> The fd of the socket
	std::vector<struct pollfd> _client_fds; ///> A vector of all the poll fd
};

}; // namespace webserv
