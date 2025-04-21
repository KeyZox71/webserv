/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/20 18:41:04 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cerrno>
#include <cmath>
#include <fcntl.h>
#include <iterator>
#include <log.hpp>
#include <netinet/in.h>
#include <poll.h>
#include <requests/default.hpp>
#include <server/default.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
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

	for (std::vector<int>::iterator it = _fds_server.begin();
		 it != _fds_server.end(); it++) {
		fd.fd = *it;
		fd.events = POLLIN;
		_client_fds.push_back(fd);
		_client_data.push_back(NULL);
	}

	// to add signal instead of 727
	while (727) {
		if (poll(_client_fds.data(), _client_fds.size(), -1) < 0) {
			std::stringstream str;
			str << "poll failed : ";
			str << strerror(errno);
			_log->error(str.str());
			continue;
		}

		for (std::vector<struct pollfd>::iterator it = _client_fds.begin();
			 it != _client_fds.end(); it++) {
			struct sockaddr_in client_addr;
			socklen_t		   addrlen = sizeof(client_addr);
			int				   client_fd =
				accept((*it).fd, (struct sockaddr *)&client_addr, &addrlen);

			if (client_fd < 0) {
				std::stringstream str;
				str << "Accept failed: ";
				str << strerror(errno);
				_log->error(str.str());
				continue;
			}

			if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1) {
				std::stringstream str;
				str << "Failed to set non-blocking mode: ";
				str << strerror(errno);
				_log->error(str.str());
				close(client_fd);
				continue;
			}

			pollfd pfd;
			pfd.fd = client_fd;
			pfd.events = POLLIN | POLLOUT;
			_client_fds.push_back(pfd);
			_client_data.push_back(new sockaddr_in(client_addr));
		}

		for (size_t i = 0; i < _client_fds.size(); ++i) {
			if (_client_fds[i].revents & POLLIN) {
				if (!_handle_client(_client_fds[i], _client_data[i])) {
					close(_client_fds[i].fd);
					_client_fds.erase(_client_fds.begin() + i);
					delete _client_data[i];
					_client_data.erase(_client_data.begin() + i);
					i--;
				}
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
