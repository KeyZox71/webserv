/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:07:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/12 01:19:52 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/HttpRequest.hpp>
/*
HttpRequest parseRequest(const std::string &rawRequest)
{
	std::istringstream	stream(rawRequest);
	HttpRequest			request;
	std::string			line;

	if (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		line_stream >> request.method >> request.target >> request.protocol;
		request.target.insert(request.target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r")
	{
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos)
		{
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			request.headers[key] = value;
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	request.body = body_stream.str();

	std::cout << "method: " << request.method << std::endl;
	std::cout << "target: " << request.target << std::endl;
	std::cout << "protocol: " << request.protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it)
	  std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "-- body --" << std::endl << request.body << std::endl;

	return request;
}
*/

/* GET response creation
HttpResponse::HttpResponse(const HttpRequest &request): _protocol(request.protocol)
{
	std::ifstream file(request.target.c_str(), std::ios::binary);
	if (file)
	{
		this->_status_code = 200;
		this->_status_text = "OK";
		this->_headers["Content-Type"] = "text/html";
		this->_body = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}
	else
	{
		// TODO: replace with the new predefined array of response
		this->_status_code = 404;
		this->_status_text = "Not Found";
		this->_headers["Content-Type"] = "text/html";
		this->_body = "<html><body>nuh uh, get 404'd >:D</body></html>";
	}
}
*/

/*
class IRequest: public http::IMessage {
public:
	virtual void			parse(const http::IRequest &request) = 0;
	virtual http::Response	execute(void) = 0;

	//std::string	str(void) const;

	std::string	getProtocol(void) const;
	size_t		getStatusCode(void) const;
	std::string	getStatusText(void) const;

	void	setProtocol(std::string const protocol);
	void	setStatusCode(size_t const status_code);
	void	setStatusText(std::string const status_text);

private:
	std::string	_method;
	std::string	_target;
	std::string	_protocol;

};
*/

using namespace webserv;

void	http::IRequest::parse(http::IRequest const &request) { (void) request; }
http::Response	execute(void) { return (http::Response()); }

std::string	http::IRequest::getMethod(void) const
{
	return (this->_method);
}

std::string	http::IRequest::getTarget(void) const
{
	return (this->_target);
}

std::string	http::IRequest::getProtocol(void) const
{
	return (this->_protocol);
}

void	http::IRequest::setMethod(std::string const method)
{
	this->_method = method;
}

void	http::IRequest::setTarget(std::string const target)
{
	this->_target = target;
}

void	http::IRequest::setProtocol(std::string const protocol)
{
	this->_protocol = protocol;
}
