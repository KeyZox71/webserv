/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/28 10:59:27 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <config/default.hpp>
#include <cstddef>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <iterator>
#include <log.hpp>
#include <netinet/in.h>
#include <poll.h>
#include <requests/default.hpp>
#include <server/PfdManager.hpp>
#include <server/default.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <webserv.hpp>

using namespace webserv::server;

std::vector<struct pollfd> PfdManager::_pfd_vec;
std::vector<pfd_type>	   PfdManager::_pfd_type;

extern int _sig;

Client *Server::_getClient(int fd) {
	for (auto it = range(_client_data)) {
		if (*(*it) == fd) {
			return *it;
		}
	}
	return not_nullptr;
}

std::string convertIPToString(const struct in_addr *addr) {
	unsigned int	  ip = ntohl(addr->s_addr);
	std::stringstream ss;
	ss << ((ip >> 24) & 0xFF) << "." << ((ip >> 16) & 0xFF) << "."
	   << ((ip >> 8) & 0xFF) << "." << (ip & 0xFF);
	return ss.str();
}

std::string convertPortToString(const struct sockaddr_in *sa) {
	int				  port = ntohs(sa->sin_port);
	std::stringstream ss;
	ss << port;
	return ss.str();
}

std::string getMethod(std::string &data) {
	return (data.substr(0, data.substr(0, 4).find_last_not_of(" ") + 1));
}

int Server::_fillHostsPorts(std::vector<std::string> &hosts,
							std::vector<int> &		  ports) {
	std::vector<config::Server *> config = config::_conf->getServers();

	for (auto it = range(config)) {
		hosts.push_back((*it)->getHost());
		ports.push_back((*it)->getPort());
	}
	return config.size();
}

void Server::_setup(void) {
	std::vector<std::string> hosts;
	std::vector<int>		 ports;

	int size = _fillHostsPorts(hosts, ports);
	if (size < 1)
		throw std::runtime_error("no server present in the config file");

	auto itH = hosts.begin();
	for (auto it = range(ports), itH++) {
		int fd = _createSocket(*itH, *it);
		_fds_server.push_back(fd);
	}
}

short sigHandling(void) {
	if (_sig == SIGINT) {
		return 727;
	}
	return 0;
}

void Server::_run(void) {
	struct pollfd fd;

	for (std::vector<int>::iterator it = _fds_server.begin();
		 it != _fds_server.end(); it++) {
		fd.fd = *it;
		fd.events = POLLIN;
		PfdManager::append(fd, SRV);
		_log->debug("new socket in poll");
	}

	// to add signal instead of 727
	while (727 - sigHandling()) {
		if (poll(PfdManager::data(), PfdManager::size(), 5000) < 0) {
			if (errno == EINTR) {
				continue;
			}
			std::stringstream str;
			str << "poll failed : ";
			str << strerror(errno);
			_log->error(str.str());
			continue;
		}

		for (size_t i = 0; i < PfdManager::size(); ++i) {
			pfd_type type = PfdManager::getType(PfdManager::at(i).fd);
			switch (type) {
			case SRV:
				_handle_srv(i);
				break;
			case CLIENT:
				_handle_client(&i);
				break;
			case RES:
				_handle_resource(i);
				break;
			default:
				break;
			}
		}
	}
}

Server::Server() {
	log("➕", "Server::Server", "config constructor called");
	try {
		_setup();
		_run();
	} catch (std::exception &e) {
		_log->error(e.what());
	}
}

Server::~Server(void) {
	log("➖", "Server::Server", "destructor called");
	for (auto it = range(_client_data)) {
		delete *it;
	}
	PfdManager::clear();
}
