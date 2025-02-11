/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:48:22 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/11 13:50:43 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __WEBSERV_REQUESTS_DEFAULT_HPP__
# define __WEBSERV_REQUESTS_DEFAULT_HPP__

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "default.hpp"
#include "requests/HttpRequest.hpp"
#include "requests/HttpResponse.hpp"

namespace webserv {
namespace http {

class IMessage {
public:
	virtual std::map<std::string, std::string>	getHeaders(void) const;
	virtual std::string							getBody(void) const;

	virtual void	setHeaders(std::map<std::string, std::string> const headers);
	virtual void	setBody(std::string const body);

	virtual void	addHeader(std::string const name, std::string const value);
	virtual void	rmHeader(std::string const name);

protected:
	std::map<std::string, std::string>	_headers;
	std::string							_body;

};

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_DEFAULT_HPP__
