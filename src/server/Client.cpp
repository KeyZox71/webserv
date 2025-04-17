/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:12:41 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/17 18:48:17 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/Client.hpp>

using namespace server;

/*class Client {
  public:
	Client(int, sockaddr_in, config::Config *);
	~Client(void);

	void	answer(void);

  private:
	void getRequest(void);

	int				   _fd;
	struct sockaddr_in _client_addr;
	http::IRequest	  *_request;
	http::Response	  *_response;
	config::Server	  *_conf;
	std::string		  _request_method;
};*/

Client::Client(int fd, sockaddr_in socket, config::Server *conf, Logger *log)
{
	this->_fd = fd;
	this->_client_addr = socket;
	this->_conf = conf;
	this->_log = log;

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
