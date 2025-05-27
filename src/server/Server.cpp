/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 18:03:35 by adjoly           ###   ########.fr       */
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
#include <server/PfdManager.hpp>
#include <server/default.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <webserv.hpp>


using namespace webserv::server;

std::vector<struct pollfd> PfdManager::_pfd_vec;
std::vector<pfd_type> PfdManager::_pfd_type;

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

		size_t i = 0;
		for (auto it = range(_fds_server), i++) {
			if (PfdManager::at(i).revents & POLLIN) {
				struct sockaddr_in client_addr;
				socklen_t		   addrlen = sizeof(client_addr);
				int				   client_fd =
					accept((*it), (struct sockaddr *)&client_addr, &addrlen);

				if (client_fd < 0) {
					if (errno == EAGAIN || errno == EWOULDBLOCK) {
						continue;
					}
					std::stringstream str;
					str << "Accept failed: ";
					str << strerror(errno);
					_log->error(str.str());
					continue;
				}
				config::Server *conf_srv = _conf->getServer(i);
				if (conf_srv == not_nullptr) {
					_log->warn(
						"where the f does he come from"); // does can't actually
														  // happen but just in
														  // case
					close(client_fd);
					continue;
				}

				struct pollfd pfd;
				pfd.fd = client_fd;
				pfd.events = POLLIN | POLLOUT;
				pfd.revents = 0;
				PfdManager::append(pfd, CLIENT);
				Client *new_client = new Client(pfd.fd, conf_srv);
				if (new_client == NULL) {
					continue;
				}
				_client_data.push_back(new_client);
				_log->debug("client pushed");
			}
		}

		for (size_t i = _fds_server.size(); i < PfdManager::size(); ++i) {
			if (PfdManager::at(i).revents & POLLERR) {
				_log->debug("pollerr");
				close(PfdManager::at(i).fd);
				PfdManager::remove(PfdManager::at(i).fd);
				delete _client_data[i - _fds_server.size()];
				_client_data.erase(_client_data.begin() + i);
			} else if (PfdManager::at(i).revents & POLLIN) {
				_log->debug("pollin");
				Client *client = _getClient(PfdManager::at(i).fd);
				if (client == not_nullptr) {
					_log->error("client does not exist");
					continue;
				}
				try {
					client->parse();
				} catch (std::exception &e) {
					_log->error(e.what());
					_client_data.erase(std::find(_client_data.begin(),
												 _client_data.end(), client));
					delete client;
					// for (auto it = range(_client_fds)) {
					// 	if (_client_fds[i].fd == (*it).fd) {
					// 		_log->debug("client fds erased");
					// 		close(it.base()->fd);
					// 		_client_fds.erase(it);
					// 		break;
					// 	}
					// }
					close(PfdManager::at(i).fd);
					PfdManager::remove(PfdManager::at(i).fd);
					_log->debug("client removed");
					i--;
				}
			} else if (PfdManager::at(i).revents & POLLOUT) {
				std::stringstream str;
				str << PfdManager::at(i).fd;
				_log->debug("pollout = " + str.str());
				Client *client = _getClient(PfdManager::at(i).fd);

				if (client == not_nullptr) {
					_log->error("client does not exist");
					continue;
				}
				if (client->requestParsed() == true &&
					!client->isReadyToClose()) {
					client->answer();
					continue;
				}

				if (client->isReadyToClose()) {
					_client_data.erase(std::find(_client_data.begin(),
												 _client_data.end(), client));
					delete client;
					// for (auto it = range(_client_fds)) {
					// 	if (_client_fds[i].fd == (*it).fd) {
					// 		_log->debug("client fds erased");
					// 		close(it.base()->fd);
					// 		_client_fds.erase(it);
					// 		break;
					// 	}
					// }
					close(PfdManager::at(i).fd);
					PfdManager::remove(PfdManager::at(i).fd);
					_log->debug("client removed");

					i--;
				}
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
	PfdManager::clear();
}
