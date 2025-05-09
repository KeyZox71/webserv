/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IMessage.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 22:13:38 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/09 11:08:24 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

namespace webserv {
namespace http {

class IMessage {
  public:
	virtual std::map<std::string, std::string> getHeaders(void) const;
	virtual std::string						   getHeader(const std::string) const;
	virtual std::string						   getBody(void) const;

	virtual ~IMessage() {}

	virtual void setHeaders(std::map<std::string, std::string> const headers);
	virtual void setBody(std::string const body);

	virtual void addHeader(std::string const key, std::string const value);
	virtual void rmHeader(std::string const key);

	virtual std::string str(void) const = 0;

  protected:
	std::map<std::string, std::string> _headers;
	std::string						   _body;

	static const std::map<int, std::string> _response_status_codes;
};

} // namespace http
} // namespace webserv
