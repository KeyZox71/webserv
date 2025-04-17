/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/17 14:35:01 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <log.hpp>
#include <netinet/in.h>
#include <requests/default.hpp>
#include <server/default.hpp>
#include <sstream>
#include <stdexcept>
#include <sys/socket.h>
#include <webserv.hpp>

using namespace webserv;

std::string getMethod(std::string &data) {
	return (data.substr(0, data.substr(0, 4).find_last_not_of(" ") + 1));
}

void Server::_handle_client(int fd) {
	std::string received_data;
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_received;
	do {
		std::memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(fd, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_received == -1) {
			_log->error("failed to receive request");
			continue;
		}
		received_data += std::string(buffer, bytes_received);
	} while (buffer[bytes_received]);

	std::string response;

	if (getMethod(received_data) == "GET") {
		_log->info("get request received");
		http::Get request(received_data);

		response = request.execute().str();
	} else if (getMethod(received_data) == "DELETE") {
		_log->info("delete request received");
		http::Delete request(received_data);

		response = request.execute().str();
	} else if (getMethod(received_data) == "POST") {
		_log->info("post request received");
		http::Post request(received_data);

		response = request.execute().str();
	} else {
		response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: "
				   "text/html\r\n\r\n<html><body><h1>501 Not "
				   "Implemented</h1></body></html>";
	}

	send(fd, response.c_str(), response.length(), 0);
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
	}

	while (727) {
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
