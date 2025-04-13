/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:11:40 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/13 11:53:40 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cmath>
#include <log.hpp>
#include <netinet/in.h>
#include <server/default.hpp>
#include <stdexcept>
#include <sys/socket.h>
#include <webserv.hpp>
#include <requests/default.hpp>

using namespace webserv;

std::string getMethod(std::string &data)
{
	return (data.substr(0, data.substr(0, 4).find_last_not_of(" ") + 1));
}

void Server::_handle_client(int fd)
{
	(void) fd;

	std::string received_data;
	char buffer[BUFFER_SIZE];
	ssize_t bytes_received;
	do
	{
		std::memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(fd, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_received == -1)
		{
			_log->error("failed to receive request");
			continue;
		}
		received_data += std::string(buffer, bytes_received);
	}
	while (buffer[bytes_received]);

	std::string	response;

	if (getMethod(received_data) == "GET")
	{
		_log->info("get request received");
		http::Get	request(received_data);

		response = request.execute().str();
	}
	else if (getMethod(received_data) == "DELETE")
	{
		_log->info("delete request received");
		http::Delete	request(received_data);

		response = request.execute().str();
	}
	else if (getMethod(received_data) == "POST")
	{
		_log->info("post request received");
		http::Post	request(received_data);

		response = request.execute().str();
	}
	else
	{
		response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
	}

    send(fd, response.c_str(), response.length(), 0);
}

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

void Server::_run(void)
{
	struct pollfd fd;

	fd.fd = _fd_server;
	fd.events = POLLIN;
	_client_fds.clear();
	_client_fds.push_back(fd);

	int nbr_client = 0;

	while (727)
	{
		int ret = poll(_client_fds.data(), nbr_client + 1, -1);
		if (ret < 0)
		{
			close(_fd_server);
			throw std::runtime_error("poll failed :(");
		}

		if (_client_fds[0].revents & POLLIN)
		{
			struct sockaddr_in client_addr;
			socklen_t		   addrlen = sizeof(client_addr);
			int				   client_fd = accept(_fd_server, (struct sockaddr *)&client_addr, &addrlen);
			if (client_fd < 0)
			{
				_log->error("accept failed");
				continue;
			}
			struct pollfd client_pollfd;
			client_pollfd.fd = client_fd;
			client_pollfd.events = POLLIN;
			_client_fds.push_back(client_pollfd);
			++nbr_client;

			// if (nbr_client) TODO do we need a max client probably not :D
		}
		for (int i = 1; i <= nbr_client; )
		{
			if (_client_fds[i].revents & POLLIN)
			{
				_handle_client(_client_fds[i].fd);
				close(_client_fds[i].fd);
				_client_fds[i] = _client_fds[nbr_client--];
                _client_fds.pop_back();
			}
			else
				++i;
		}
	}
}

Server::Server(config::Server *conf) : _conf(conf) {
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
	close(_fd_server);
}
