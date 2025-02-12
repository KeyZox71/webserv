/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:00 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/12 01:21:31 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __WEBSERV_REQUESTS_HTTP_REQUEST_HPP__
# define __WEBSERV_REQUESTS_HTTP_REQUEST_HPP__

#include <fstream>
#include <sstream>
#include <iostream>

#include <requests/HttpIMessage.hpp>
#include <requests/HttpResponse.hpp>

namespace webserv {
namespace http {

class IRequest: public http::IMessage {
public:
	virtual void			parse(http::IRequest const &request) = 0;
	virtual http::Response	execute(void) = 0;

	//std::string	str(void) const;

	std::string	getMethod(void) const;
	std::string	getTarget(void) const;
	std::string	getProtocol(void) const;

	void	setMethod(std::string const method);
	void	setTarget(std::string const target);
	void	setProtocol(std::string const protocol);

private:
	std::string	_method;
	std::string	_target;
	std::string	_protocol;

};

class Get: public http::IRequest {
public:
	Get(void);
	Get(std::string &data);

	void	parse(const http::IRequest &request);

	http::Response	execute(void);

};

class Post: public http::IRequest {
public:
	Post(void);
	Post(std::string &data);

	void	parse(const http::IRequest &request);

	http::Response	execute(void);

};

class Delete: public http::IRequest {
public:
	Delete(void);
	Delete(std::string &data);

	void	parse(const http::IRequest &request);

	http::Response	execute(void);

};

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_HTTP_REQUEST_HPP__
