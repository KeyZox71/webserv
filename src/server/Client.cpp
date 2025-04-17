/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:12:41 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/17 14:25:50 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/Client.hpp>

using namespace server;

Client::Client(int fd, sockaddr_in socket, config::Servr *conf, Logger *log): _fd(fd), _client_addr(socker), _conf(conf), _log(log)
{
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


	this->getRequest(request_str);
}

void	Client::_getRequest(std::string request_str)
{
	std::string method = request_str.substr(0, request_str.substr(0, 4).find_last_not_of(" ") + 1);

	if (method == "GET")
	{
		_log->info("get request received");
		this->_request = new http::Get(request_str);
	}
	else if (method == "DELETE")
	{
		_log->info("delete request received");
		this->_request = new http::Delete(request_str);
	}
	else if (method == "POST")
	{
		_log->info("post request received");
		this->_request = new http::Post(request_str);
	}
	else
	{
		_log->info("unsupported request received");
		this->_request = new http::Get();
		this->_request->setMethod("501");
	}
}

void	Client::answer(void)
{
	std::string response;
	
	if (this->_request == "GET" || this->_request == "DELETE" || this->_request == "POST")
		response = this->_request.execute().str();
	else
		response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
	send(this->_fd, response.c_str(), response.length(), 0);
}


Client::~Client(void)
{
	delete this->_request;
	delete this->_response;
}
