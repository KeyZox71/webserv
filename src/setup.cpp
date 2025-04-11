/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/11 19:22:50 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log.hpp"
#include "server/default.hpp"
#include <cmath>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <webserv.hpp>

using namespace webserv;

void Server::_setup(void) {
	_fd_server = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (_fd_server == -1)
		throw std::runtime_error("error at socket setup");

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(_conf->getPort());

	if (bind(_fd_server, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(_fd_server);
		throw std::runtime_error("error when binding address");
	}

	if (listen(_fd_server, SOMAXCONN) < 0) {
		close(_fd_server);
		throw std::runtime_error("error when listening");
	}
}

void Server::_run(void) {
	_client_fds[0].fd = _fd_server;
	_client_fds[0].events = POLLIN;

	int nbr_client = 0;

	while (727) {
		int ret = poll(_client_fds.data(), nbr_client + 1, -1);
		if (ret < 0) {
			close(_fd_server);
			throw std::runtime_error("poll failed :(");
		}

		if (_client_fds[0].revents & POLLIN) {
			struct sockaddr_in client_addr;
			socklen_t		   addrlen = sizeof(client_addr);
			int				   client_fd =
				accept(_fd_server, (struct sockaddr *)&client_addr, &addrlen);
			if (client_fd < 0) {
				_log->error("accept failed");
				continue;
			}
			//if (nbr_client ) TODO do we need a max client probably not
		}
		for (int i = 1; i <= nbr_client; ++i) {
			if (_client_fds[i].revents & POLLIN) {
				_handle_client(i);
				close(_client_fds[i].fd);
				_client_fds[i] = _client_fds[nbr_client--];
			}
		}
	}
}
