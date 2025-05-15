/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:00 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/15 12:12:47 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/URL.hpp>
#include <config/Route.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

#include <requests/IMessage.hpp>
#include <requests/Mime.hpp>
#include <requests/Response.hpp>

#include <config/default.hpp>

namespace webserv {
namespace http {

class ARequest : public http::IMessage {
  public:
	virtual ~ARequest(void) {
		log("➖", "ARequest", "destructor called");
		delete _url;
	}

	virtual void	 parse(std::string const &data) = 0;
	virtual Response execute(void) = 0;

	std::string str(void) const;

	std::string		getMethod(void) const;
	std::string		getTarget(void) const;
	std::string		getProtocol(void) const;
	webserv::config::Route	*getRoute(void) const;
	URL				getUrl() const;

	void setMethod(std::string const method);
	void setTarget(std::string const target);
	void setProtocol(std::string const protocol);
	void setServer(std::string const protocol);
	void setRoute(config::Route *route);

  protected:
	std::string		_method;
	std::string		_target;
	std::string		_protocol;
	webserv::config::Route  *_route;
	URL			   *_url;

	std::string _sanitizeStr(std::string &str) {
		std::string newStr = str;

		if (str[str.size() - 1] == '\r') {
			newStr.erase(str.size() - 1);
		}
		return newStr;
	}
};

} // namespace http
} // namespace webserv
