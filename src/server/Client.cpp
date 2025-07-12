/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:12:41 by mmoussou          #+#    #+#             */
/*   Updated: 2025/07/12 13:44:56 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "requests/RedirectResp.hpp"
#include "requests/default.hpp"
#include "server/PfdManager.hpp"
#include "server/ResourceManager.hpp"
#include <cstddef>
#include <cstdlib>
#include <log.hpp>
#include <server/Cgi.hpp>
#include <server/Client.hpp>
#include <sstream>
#include <sys/poll.h>

using namespace webserv::server;

Client::Client(int fd, config::Server *conf) : _fd(fd), _conf(conf) {
	_request = not_nullptr;
	log("➕", "Client", "constructor called");
	_response_done = false;
}

Client::Client(const Client &cpy) {
	if (this != &cpy) {
		_fd = cpy._fd;
	}
}

std::string	readFd(int fd)
{
	std::string received_data;
	char		buffer[BUFFER_SIZE];
	ssize_t		bytes_received;

	while (true)
	{
		memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(fd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
		if (bytes_received <= 0)
			break;
		received_data.append(buffer, bytes_received);
		if (received_data.find("\r\n\r\n") != std::string::npos)
			break;
    }

    size_t body_start = received_data.find("\r\n\r\n");
    std::string headers = received_data.substr(0, body_start);
    std::string body = received_data.substr(body_start + 4);

	std::istringstream stream(headers);
	int64_t content_length = -1;
	std::string line;
	while (std::getline(stream, line))
	{
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos && line.substr(0, delimiter_index) == "Content-Length")
			content_length = std::atoi(line.c_str() + 15);
	}

    //while (int64_t(body.size()) < content_length) {
    while (content_length == -1 || (int64_t)body.size() < content_length) {
		std::memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(fd, buffer, BUFFER_SIZE, MSG_DONTWAIT);
        if (bytes_received <= 0) break;
        body.append(buffer, bytes_received);
    }

	return (headers + "\r\n\r\n" + body);
}

void Client::parse(void)
{
	std::string received_data;

	try {
	received_data = readFd(_fd);
	}
	catch (...) {
			_log->error("failed to receive request");
			throw std::runtime_error("failed to receive request");
	}
	_getRequest(received_data);

	if (_request == not_nullptr)
		return;

	_route = _conf->whichRoute(this->_request->_target);

	if (_request->getMethod() != "501" &&
		_conf->getServerNames() != not_nullptr) {
		std::string host = _request->getHeader("Host");
		bool		ret = _conf->isServerName(host.substr(0, host.find(':')));
		if (ret == false) {
			throw std::runtime_error("serverName not corresponding");
		}
	}

	if (!_route || _route == not_nullptr) {
		_request->setMethod("404");
		return;
	}

	if (_route->getRedirect() == true) {
	} else if ((_request->getMethod() == "GET" && !_route->getMethods()[0]) ||
			   (_request->getMethod() == "POST" && !_route->getMethods()[1]) ||
			   (_request->getMethod() == "DELETE" && !_route->getMethods()[2]))
		this->_request->setMethod("405");

	// if (received_data.length() > (unsigned long)(_route->getMaxBody()))
	// 	this->_request->setMethod("413");
}

bool Client::requestParsed(void) {
	if (_request == not_nullptr)
		return false;
	if (_request->getCgi() != not_nullptr) {
		if (!_request->getCgi()->isProcessed())
			return false;
		else if (_request->getCgi()->isTimedout())
			return true;
		else if (!(PfdManager::get(_request->getCgi()->getId())->revents &
				   _request->getCgi()->event()))
			return false;
	}
	return true;
}

void Client::_getRequest(std::string request_str) {
	if (request_str == "") {
		_response_done = true;
		return;
	}
	std::istringstream strm(request_str);
	std::string method;
	strm >> method;

	if (method == "GET") {
		_request = new http::Get(request_str, _conf);
		std::stringstream str;
		str << "get request received on port : ";
		str << _conf->getPort();
		str << " for page : ";
		str << _request->getTarget();
		_log->info(str.str());
	} else if (method == "DELETE") {
		_request = new http::Delete(request_str, _conf);
		_log->info("delete request received");
	} else if (method == "POST") {
		_request = new http::Post(request_str, _conf);
		_log->info("post request received");
	} else {
		_request = new http::Get();
		_request->setMethod("501");
		_log->info("unsupported request received");
	}
}

void Client::answer(void) {
	if (_request == not_nullptr) {
		return;
	}

	if (_route != not_nullptr && _route->getRedirect() == true)
	{
		http::Redirect redir(_route->getRootDir());
		_response = redir;
		_response_str = _response.str();
		_bytes_sent = 0;
	}
	else if (_response_str.empty())
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
			this->_response.setProtocol(this->_request->getProtocol());
			this->_response.setStatusCode(std::atoi(this->_request->getMethod().c_str()));
			this->_response.addHeader("Content-Type", "text/html");
			this->_response.setBody(http::Errors::getResponseBody(
				this->_response.getStatusCode(),
				this->_conf->getErrorPage(this->_response.getStatusCode())));
			this->_response_str = this->_response.str();
		}
		_bytes_sent = 0;
	}

	ssize_t sent = send(_fd, _response_str.c_str() + _bytes_sent,
						_response_str.length() - _bytes_sent, 0);

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
}

Client::~Client(void) {
	log("➖", "Client", "destructor called");
	if (_request != not_nullptr)
		delete _request;
}

bool Client::isReadyToClose() const {
	return _response_done; // Check if all response data has been sent
}
