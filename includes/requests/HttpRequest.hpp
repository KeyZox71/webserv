/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:00 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/11 13:49:50 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __WEBSERV_REQUESTS_HTTP_REQUEST_HPP__
# define __WEBSERV_REQUESTS_HTTP_REQUEST_HPP__

#include "requests/default.hpp"

namespace webserv {
namespace http {

class IRequest: public http::IMessage {
public:
	virtual void	parse(const http::IRequest &request) = 0;
	virtual http::Response	execute(void) = 0;

	std::string	str(void) const;

	std::string							getProtocol(void) const;
	size_t								getStatusCode(void) const;
	std::string							getStatusText(void) const;

	void	setProtocol(std::string const protocol);
	void	setStatusCode(size_t const status_code);
	void	setStatusText(std::string const status_text);

private:
	std::string	_method;
	std::string	_target;
	std::string	_protocol;

};

class Get: public http::IRequest {
public:
	Get(void);
	Get(std::string &data);
	~Get(void);

	void	parse(const http::IRequest &request);

	http::Response	execute(void);

};

class Post: public http::IRequest {
public:
	Post(void);
	Post(std::string &data);
	~Post(void);

	void	parse(const http::IRequest &request);

	http::Response	execute(void);

};

class Delete: public http::IRequest {
public:
	Delete(void);
	Delete(std::string &data);
	~Delete(void);

	void	parse(const http::IRequest &request);

	http::Response	execute(void);

};

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_HTTP_REQUEST_HPP__
