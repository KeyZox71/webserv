/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:21:20 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/23 14:36:47 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>

#include <requests/HttpIMessage.hpp>

typedef unsigned int uint;

namespace webserv {
namespace http {

class Response: public http::IMessage {
public:
	Response(void);
	~Response(void);

	std::string	getProtocol(void) const;
	uint		getStatusCode(void) const;
	std::string	getStatusText(void) const;

	void	setProtocol(std::string const protocol);
	void	setStatusCode(uint const status_code);

	std::string	str(void) const;

private:
	std::string	_protocol;
	uint		_status_code;
	std::string	_status_text;

};

} // -namespace http
} // -namespace webserv
