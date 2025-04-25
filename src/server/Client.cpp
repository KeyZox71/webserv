/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:12:41 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/25 13:21:42 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <log.hpp>
#include <server/Client.hpp>

using namespace server;

Client::Client(void) {}

Client::Client(struct pollfd *pfd, sockaddr_in socket, config::Config *conf)
	: _pfd(pfd), _client_addr(socket), _Gconf(conf) {
	log("➕", "Client", "constructor called");
}

void Client::parse(void) {
	std::string received_data;
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_received;
	do {
		std::memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(_pfd->fd, buffer, BUFFER_SIZE - 1, 0);
		if (bytes_received == -1) {
			_log->error("failed to receive request");
			throw std::runtime_error("failed to receive request");
		}
		received_data += std::string(buffer, bytes_received);
	} while (buffer[bytes_received]);

	this->_getRequest(received_data);

	this->_conf = _Gconf->getServer(this->_request->getHeaders()["Host"]);

	// if (received_data.length > (get max_body_size from Route corresponding) )
	//	throw error
}

void Client::_getRequest(std::string request_str) {
	std::string method = request_str.substr(
		0, request_str.substr(0, 4).find_last_not_of(" ") + 1);

	if (method == "GET") {
		this->_request = new http::Get(request_str);
		_log->info("get request received");
	} else if (method == "DELETE") {
		this->_request = new http::Delete(request_str);
		_log->info("delete request received");
	} else if (method == "POST") {
		this->_request = new http::Post(request_str);
		_log->info("post request received");
	} else {
		this->_request = new http::Get();
		this->_request->setMethod("501");
		_log->info("unsupported request received");
	}
	// set target to correct target with the conf
}

void Client::answer(void) {
	(void)_client_addr;
	std::string response;

	if (this->_request->getMethod() == "GET" ||
		this->_request->getMethod() == "DELETE" ||
		this->_request->getMethod() == "POST")
		response = this->_request->execute().str();
	else
		response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: "
				   "text/html\r\n\r\n<html><body><h1>501 Not "
				   "Implemented</h1></body></html>";
	send(_pfd->fd, response.c_str(), response.length(), 0);
}

Client::~Client(void) {
	log("➖", "Client", "destructor called");
	delete (http::Get *)(this->_request);
}
