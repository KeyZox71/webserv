/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:21:20 by mmoussou          #+#    #+#             */
/*   Updated: 2025/03/19 02:09:01 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __WEBSERV_REQUESTS_HTTP_RESPONSE_HPP__
# define __WEBSERV_REQUESTS_HTTP_RESPONSE_HPP__

#include <sstream>

#include <requests/HttpIMessage.hpp>

namespace webserv {
namespace http {

class Response: public http::IMessage {
public:
	Response(void);

	std::string	getProtocol(void) const;
	size_t		getStatusCode(void) const;
	std::string	getStatusText(void) const;

	void	setProtocol(std::string const protocol);
	void	setStatusCode(size_t const status_code);

	std::string	str(void) const;

private:
	std::string	_protocol;
	size_t		_status_code;
	std::string	_status_text;

};

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_HTTP_RESPONSE_HPP__
