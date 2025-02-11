/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpIMessage.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:13:38 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/11 22:20:53 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __WEBSERV_REQUESTS_HTTP_IMESSAGE_HPP__
# define __WEBSERV_REQUESTS_HTTP_IMESSAGE_HPP__

#include <map>
#include <string>

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

	virtual std::string	str(void) const = 0;

protected:
	std::map<std::string, std::string>	_headers;
	std::string							_body;

};

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_HTTP_IMESSAGE_HPP__
