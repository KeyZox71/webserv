/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:28:31 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/08 01:11:40 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/HttpResponse.hpp>
#include <requests/Errors.hpp>

/*
- do a map of all the status_text and get it from here, not storing them
- get error pages from an array of predefined response, that would be modified by the config
*/

// tmp, need to be cleaned
#include <iostream>

using namespace webserv;

http::Response::Response(void)
{
}

std::string	http::Response::str(void) const
{
	std::ostringstream response;

	response << this->_protocol << " " << this->_status_code << " " << this->_status_text;
	response << "\r\n";

	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		response << it->first << ": " << it->second << "\r\n";

	response << "\r\n";
	response << this->_body;

	//std::cout << "------------ RESPONSE -------------" << std::endl << response.str();
	return (response.str());
}

std::string	http::Response::getProtocol(void) const
{
	return (this->_protocol);
}

uint		http::Response::getStatusCode(void) const
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

void	http::Response::setStatusCode(uint const status_code)
{
	this->_status_code = status_code;
	this->_status_text = Errors::message[status_code];
}
