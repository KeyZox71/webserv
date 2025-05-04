/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:28:31 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/04 13:31:10 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>
#include <requests/default.hpp>

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

http::Response::~Response(void)
{
}

std::string	http::Response::str(void) const
{
	std::ostringstream response;

	response << this->_protocol << " " << this->_status_code << " " << this->_status_text;
	response << "\r\n";

	for (auto it = range(_headers))
		response << it->first << ": " << it->second << "\r\n";

	response << "\r\n";
	response << this->_body;

	std::cout << "------------ RESPONSE -------------" << std::endl << response.str() << std::endl;
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
