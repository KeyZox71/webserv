/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/18 09:19:14 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <iterator>
#include <log.hpp>
#include <netinet/in.h>
#include <poll.h>
#include <requests/default.hpp>
#include <server/default.hpp>
#include <stdexcept>
#include <sys/socket.h>
#include <webserv.hpp>

using namespace webserv;

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
							std::vector<int>		 &ports) {
	std::vector<config::Server *> *config = _conf->getServers();

	for (std::vector<config::Server *>::iterator it = config->begin();
		 it != config->end(); it++) {
		hosts.push_back((*it)->getHost());
		ports.push_back((*it)->getPort());
	}
	return config->size();
}

void Server::_setup(void) {
	std::vector<std::string> hosts;
	std::vector<int>		 ports;

	int size = _fillHostsPorts(hosts, ports);
	if (size < 1)
		throw std::runtime_error("no server present in the config file");

	std::vector<std::string>::iterator itH = hosts.begin();
	for (std::vector<int>::iterator itP = ports.begin(); itP != ports.end();
		 itP++, itH++) {
		try {
			int fd = _createSocket(*itH, *itP);
			_fds_server.push_back(fd);
		} catch (std::exception &e) {
			throw e;
		}
	}
}

void Server::_run(void) {
	struct pollfd fd;
	int			  nbr_client = 0;

	for (std::vector<int>::iterator it = _fds_server.begin();
		 it != _fds_server.end(); it++, nbr_client++) {
		fd.fd = *it;
		fd.events = POLLIN;
		_client_fds.push_back(fd);
	}

	// to add signal instead of 727
	while (727) {
		int ret = poll(_client_fds.data(), nbr_client, -1);
		if (ret < 0) {
			_log->error("poll failed :(");
			continue;
		}

		for (int i = 0; i < nbr_client; i++) {
			if (_client_fds[i].revents & POLLIN) {
				struct sockaddr_in client_addr;
				socklen_t		   addrlen = sizeof(client_addr);
				int				   client_fd =
					accept(_client_fds[i].fd, (struct sockaddr *)&client_addr,
						   &addrlen);
				if (client_fd < 0) {
					_log->error("accept failed with : " +
							   convertIPToString(&client_addr.sin_addr) + ":" +
							   convertPortToString(&client_addr));
					continue ;
				}
				_handle_client(client_fd, client_addr, _conf, _client_fds[i]);
			}
		}
	}
}

Server::Server(config::Config *conf) : _conf(conf) {
	log("➕", "Server::Server", "config constructor called");
	_log = conf->getLogger();
	_setup();
	_run();
}

Server::~Server(void) {
	log("➖", "Server::Server", "destructor called");
	for (std::vector<struct pollfd>::iterator it = _client_fds.begin();
		 it != _client_fds.end(); it++)
		close(it->fd);
}
