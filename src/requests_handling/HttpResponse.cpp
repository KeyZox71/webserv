/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:28:31 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/12 01:00:54 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requests/HttpResponse.hpp"

/*
- do a map of all the status_text and get it from here, not storing them
- get error pages from an array of predefined response, that would be modified by the config
*/

using namespace webserv;

http::Response::Response(void)
{
}

std::string	http::Response::str(void) const
{
	std::ostringstream response;

	response << this->_protocol << " " << this->_status_code << " " << this->_status_text;
	response << "\r\n";

	for (std::multimap<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		response << it->first << ": " << it->second << "\r\n";

	response << "\r\n";
	response << this->_body;

	return (response.str());
}

std::string	http::Response::getProtocol(void) const
{
	return (this->_protocol);
}

size_t		http::Response::getStatusCode(void) const
{
	return (this->_status_code);
}

std::string	http::Response::getStatusText(void) const
{
	return (this->_status_text);
}

void	http::Response::setProtocol(std::string const protocol)
{
	this->_protocol = protocol;
}

void	http::Response::setStatusCode(size_t const status_code)
{
	this->_status_code = status_code;
}

void	http::Response::setStatusText(std::string const status_text)
{
	this->_status_text = status_text;
}
