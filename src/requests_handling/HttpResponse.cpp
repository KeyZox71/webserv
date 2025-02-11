/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:28:31 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/11 10:20:12 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requests/HttpResponse.hpp"

/*std::string	_protocol;
size_t		_status_code;
std::string	_status_text;
std::map<std::string, std::string> _headers;
std::string _body;*/
/*
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
		this->_status_code = 404;
		this->_status_text = "Not Found";
		this->_headers["Content-Type"] = "text/html";
		this->_body = "<html><body>nuh uh, get 404'd >:D</body></html>";
	}
}

HttpResponse::~HttpResponse(void)
{
}

std::string	HttpResponse::str(void) const
{
	std::ostringstream response;

	response << this->_protocol << " " << this->_status_code << " " << this->_status_text;
	response << "\r\n";

	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		response << it->first << ": " << it->second << "\r\n";

	response << "\r\n";
	response << this->_body;

	return (response.str());
}*/
