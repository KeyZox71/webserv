/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:00 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/22 11:52:16 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>

#include <requests/HttpIMessage.hpp>
#include <requests/HttpResponse.hpp>

#include <config/default.hpp>

namespace webserv {
namespace http {

class IRequest: public http::IMessage {
public:
	virtual ~IRequest(void);

	virtual void			parse(std::string const &data) = 0;
	virtual http::Response	execute(void) = 0;

	std::string	str(void) const;

	std::string	getMethod(void) const;
	std::string	getTarget(void) const;
	std::string	getProtocol(void) const;
	config::Server	*getConfig(void) const;

	void	setMethod(std::string const method);
	void	setTarget(std::string const target);
	void	setProtocol(std::string const protocol);
	void	setServer(std::string const protocol);

protected:
	std::string	_method;
	std::string	_target;
	std::string	_protocol;
	config::Server	*_conf;

};

class Get: public http::IRequest {
public:
	Get(void);
	~Get(void);
	Get(std::string &data);

	void	parse(std::string const &data);

	http::Response	execute(void);

};

class Post: public http::IRequest {
public:
	Post(void);
	~Post(void);
	Post(std::string &data);

	void	parse(std::string const &data);

	http::Response	execute(void);

};

class Delete: public http::IRequest {
public:
	Delete(void);
	~Delete(void);
	Delete(std::string &data);

	void	parse(std::string const &data);

	http::Response	execute(void);

};

} // -namespace http
} // -namespace webserv
