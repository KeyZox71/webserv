/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 23:34:45 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/09 11:20:43 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requests/IMessage.hpp"
#include <requests/default.hpp>

using namespace webserv::http;

std::map<std::string, std::string> IMessage::getHeaders(void) const {
	return (this->_headers);
}

std::string IMessage::getHeader(const std::string key) const {
	std::string str;
	auto it = _headers.find(key);
	if (it == _headers.end())
		return "";
	str = it->second;
	return str;
}

std::string IMessage::getBody(void) const { return (this->_body); }

void IMessage::setHeaders(std::map<std::string, std::string> const headers) {
	this->_headers = headers;
}

void IMessage::setBody(std::string const body) { this->_body = body; }

void IMessage::addHeader(std::string const key, std::string const value) {
	this->_headers.insert(std::make_pair(key, value));
}

void IMessage::rmHeader(std::string const key) { this->_headers.erase(key); }
