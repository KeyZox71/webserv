/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/25 17:23:49 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <exception>
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

using namespace webserv::server;

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
							std::vector<int>		 &ports) {
	std::vector<config::Server *> config = _conf->getServers();

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
		_client_fds.push_back(fd);
		_log->debug("new socket in poll");
	}

	// to add signal instead of 727
	while (727 - sigHandling()) {
		if (poll(_client_fds.data(), _client_fds.size(), -1) < 0) {
			if (errno == EINTR)
				continue;
			std::stringstream str;
			str << "poll failed : ";
			str << strerror(errno);
			_log->error(str.str());
			continue;
		}

		for (auto it = range(_fds_server)) {
			struct sockaddr_in client_addr;
			socklen_t		   addrlen = sizeof(client_addr);
			int				   client_fd =
				accept((*it), (struct sockaddr *)&client_addr, &addrlen);

			if (client_fd < 0) {
				if (errno == EAGAIN || errno == EWOULDBLOCK)
					continue;
				std::stringstream str;
				str << "Accept failed: ";
				str << strerror(errno);
				_log->error(str.str());
				continue;
			}

			struct pollfd pfd;
			pfd.fd = client_fd;
			pfd.events = POLLIN | POLLOUT;
			pfd.revents = 0;
			_client_fds.push_back(pfd);
			struct pollfd *ppfd = _client_fds.data() + _client_fds.size() - 1;
			Client		  *new_client = new Client(ppfd, client_addr, _conf);
			_client_data.push_back(new_client);
		}

		for (size_t i = _fds_server.size(); i < _client_fds.size(); ++i) {
			if (_client_fds[i].revents & POLLERR) {
				 close(_client_fds[i].fd);
				_client_fds.erase(_client_fds.begin() + i);
				 delete _client_data[i - _fds_server.size()];
				_client_data.erase(_client_data.begin() + i);
			} else if (_client_fds[i].revents & POLLIN) {
				Client *client = _getClient(_client_fds[i].fd);
				if (client == not_nullptr) {
					_log->error("client does not exist");
					continue;
				}
				client->parse();
			} else if (_client_fds[i].revents & POLLOUT) {
				Client *client = _getClient(_client_fds[i].fd);
				if (client == not_nullptr) {
					_log->error("client does not exist");
					continue;
				}
				client->answer();
				_client_data.erase(std::find(_client_data.begin(),
											 _client_data.end(), client));
				delete client;
				for (auto it = range (_client_fds)) {
					if (_client_fds[i].fd == (*it).fd) {
						_client_fds.erase(it);
						break;
					}
				}
				close(_client_fds[i].fd);
			}
		}
	}
}

Server::Server(config::Config *conf) : _conf(conf) {
	log("➕", "Server::Server", "config constructor called");
	_log = conf->getLogger();
	try {
		_setup();
		_run();
	} catch (std::exception &e) {
		_log->error(e.what());
	}
}

Server::~Server(void) {
	log("➖", "Server::Server", "destructor called");
	for (std::vector<struct pollfd>::iterator it = _client_fds.begin();
		 it != _client_fds.end(); it++)
		close(it->fd);
}
