/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpIMessage.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 23:34:45 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/02 01:46:52 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/HttpIMessage.hpp>

using namespace webserv;

std::map<std::string, std::string>	http::IMessage::getHeaders(void) const
{
	return (this->_headers);
}

std::string	http::IMessage::getBody(void) const
{
	return (this->_body);
}

void	http::IMessage::setHeaders(std::map<std::string, std::string> const headers)
{
	this->_headers = headers;
}

void	http::IMessage::setBody(std::string const body)
{
	this->_body = body;
}

void	http::IMessage::addHeader(std::string const key, std::string const value)
{
	this->_headers.insert(std::make_pair(key, value));
}

void	http::IMessage::rmHeader(std::string const key)
{
	this->_headers.erase(key);
}
