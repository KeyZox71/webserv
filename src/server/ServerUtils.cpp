/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:58:42 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/30 15:25:15 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <algorithm>
#include <netinet/in.h>
#include <server/Client.hpp>
#include <server/default.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <vector>

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

	int opt = 1;
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		close(fd);
		throw std::runtime_error("setsockopt failed");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	if (!convertStringToIP(host.c_str(), &addr.sin_addr)) {
		throw std::runtime_error("ip is not of the valid format : " + host);
	}
	std::stringstream str;
	str << port;
	//_log->debug("port : " + str.str());
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
		throw std::runtime_error("listen failed for : " + host + ":" +
								 str.str());
	}

	return (fd);
}

std::vector<struct pollfd>::iterator getPfd(int fd, std::vector<struct pollfd> &s) {
	for (auto it = range(s)) {
		if ((*it).fd == fd) {
			return it;
		}
	}
	return s.end();
}
