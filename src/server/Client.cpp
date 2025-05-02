/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:12:41 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/02 14:26:32 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "requests/default.hpp"
#include <log.hpp>
#include <server/Client.hpp>
#include <sstream>

using namespace webserv::server;

Client::Client(struct pollfd *pfd, config::Server *conf)
	: _pfd(pfd), _conf(conf) {
	_request = not_nullptr;
	log("➕", "Client", "constructor called");
	_response_done = false;
}

Client::Client(const Client &cpy) {
	if (this != &cpy) {
		_pfd->fd = cpy._pfd->fd;
	}
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

	_getRequest(received_data);

	_route = _conf->whatRoute(URL(this->_request->getTarget()));
	this->_request->setRoute(_route);

	if (!this->_route || this->_route == not_nullptr) {
		this->_request->setMethod("404");
		return;
	}

	if ((this->_request->getMethod() == "GET" && !_route->getMethods()[0]) ||
		(this->_request->getMethod() == "POST" && !_route->getMethods()[1]) ||
		(this->_request->getMethod() == "DELETE" && !_route->getMethods()[2]))
		this->_request->setMethod("405");

	if (received_data.length() > (unsigned long)(_route->getMaxBody()))
		this->_request->setMethod("413");

}

bool Client::requestParsed(void) {
	if (_request == not_nullptr)
		return false;
	return true;
}

void Client::_getRequest(std::string request_str) {
	std::string method = request_str.substr(
		0, request_str.substr(0, 4).find_last_not_of(" ") + 1);

	if (method == "GET") {
		this->_request = new http::Get(request_str);
		std::stringstream str;
		str << "get request received on port : ";
		str << _conf->getPort();
		str << " for page : ";
		str << _request->getTarget();
		_log->info(str.str());
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
	if (_request == not_nullptr) {
		return;
	}

	if (_response_str.empty())
	{
		if (this->_request->getMethod() == "GET" ||
			this->_request->getMethod() == "DELETE" ||
			this->_request->getMethod() == "POST")
		{
			_response = this->_request->execute();
			_response_str = _response.str();
		}
		else
		{
			this->_response.setStatusCode(501);
			_response_str = "HTTP/1.1 501 Not Implemented\r\nContent-Type: "
					   "text/html\r\n\r\n<html><body><h1>501 Not "
					   "Implemented</h1></body></html>";
		}
		_bytes_sent = 0;
	}

	ssize_t sent = send(_pfd->fd, _response_str.c_str() + _bytes_sent, _response_str.length() - _bytes_sent, 0);

	if (sent == -1) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		_log->error("send failed: " + std::string(strerror(errno)));
		_response_done = true;
		return;
	}

	_bytes_sent += sent;

	if (_bytes_sent >= _response_str.length()) {
		_response_done = true;
		std::stringstream str;
		str << "response sent, for page : ";
		str << _request->getTarget();
		str << " with response code : ";
		str << _response.getStatusCode();
		_log->info(str.str());
	}
	

	/*std::stringstream str;
	str << "response sent, for page : ";
	str << _request->getTarget();
	str << " with response code : ";
	str << _response.getStatusCode();
	_log->info(str.str());*/
}

Client::~Client(void) {
	log("➖", "Client", "destructor called");
	delete (http::Get *)(this->_request);
}

bool Client::isReadyToClose() const {
    return _response_done;  // Check if all response data has been sent
}
