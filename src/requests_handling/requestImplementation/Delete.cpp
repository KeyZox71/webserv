/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:42:18 by adjoly            #+#    #+#             */
/*   Updated: 2025/06/24 18:02:42 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

#include <requests/default.hpp>

using namespace webserv::http;

Delete::Delete(std::string &data, config::Server *srv) {
	_url = not_nullptr;
	_srv = srv;
	this->parse(data);
}

void Delete::parse(std::string const &data) {
	std::istringstream stream(data);
	std::string		   line;

	if (std::getline(stream, line)) {
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		_method = _sanitizeStr(_method);
		_target = _sanitizeStr(_target);
		_protocol = _sanitizeStr(_protocol);
		// this->_target.insert(this->_target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r") {
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos) {
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			this->_headers.insert(std::make_pair(key, _sanitizeStr(value)));
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	_url = new URL(_target);
	std::cout << *_url << std::endl;

	/*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::endl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it =
	this->_headers.begin(); it != this->_headers.end(); ++it) std::cout <<
	it->first << ": " << it->second << std::endl; std::cout << std::endl;
	std::cout << "-- body --" << std::endl << this->_body << std::endl;
	*/
}

Response Delete::execute(void) {
	http::Response response;

	this->_target = this->_route->getRootDir() + this->_target;

	try {
		if (std::remove(this->_target.c_str()))
			throw std::runtime_error("can't remove file, FF");
		response.setProtocol(this->_protocol);
		response.setStatusCode(204);
		time_t now = std::time(NULL);
		response.addHeader("Date", std::string(std::ctime(&now)));
	} catch (...) {
		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(
			response.getStatusCode(),
			_srv->getErrorPage(response.getStatusCode())));
	}

	return (response);
}
