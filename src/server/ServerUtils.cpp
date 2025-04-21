/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:58:42 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/21 10:53:45 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Client.hpp"
#include <netinet/in.h>
#include <server/default.hpp>
#include <sys/socket.h>

using namespace webserv::server;

bool convertStringToIP(const char *ip_str, struct in_addr *addr) {
	// Split the IP string into four octets
	unsigned int a, b, c, d;
	if (sscanf(ip_str, "%u.%u.%u.%u", &a, &b, &c, &d) != 4) {
		return false;
	}

	// Check if each octet is within the valid range
	if (a > 255 || b > 255 || c > 255 || d > 255) {
		return false;
	}

	// Combine the octets into a single 32-bit address
	addr->s_addr = htonl((a << 24) | (b << 16) | (c << 8) | d);
	return true;
}

int Server::_createSocket(std::string host, int port) {
	int fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (fd == -1) {
		std::ostringstream str;
		str << port;
		throw std::runtime_error("socket binding failed for : " + host + ":" +
								 str.str());
		return -1;
	}

	if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) {
		close(fd);
		throw std::runtime_error("fcntl failed");
	}

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(fd);
		throw std::runtime_error("setsockopt failed");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	convertStringToIP(host.c_str(), &addr.sin_addr);
	addr.sin_port = htons(port);

	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		close(fd);
		std::ostringstream str;
		str << port;
		throw std::runtime_error("bind failed for : " + host + ":" + str.str());
	}

	if (listen(fd, SOMAXCONN) < 0) {
		close(fd);
		std::ostringstream str;
		str << port;
		throw std::runtime_error("listen failed for : " + host + ":" + str.str());
	}

	return (fd);
}

bool	Server::_handle_client(struct pollfd &pollfd, sockaddr_in *sock_data) {
	Client *client;

	try {
		client = new Client(pollfd.fd, sock_data, _conf);
		client->answer();
	} catch (std::exception &e) {
		_log->error(e.what());
		return false;
	}

	delete client;

	return true;
}
