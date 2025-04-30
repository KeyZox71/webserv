/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequest.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:00 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/30 09:34:49 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config/URL.hpp"
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
	config::Server *getConfig(void) const;

	void setMethod(std::string const method);
	void setTarget(std::string const target);
	void setProtocol(std::string const protocol);
	void setServer(std::string const protocol);

  protected:
	std::string		_method;
	std::string		_target;
	std::string		_protocol;
	config::Server *_conf;
	URL			   *_url;
};

} // namespace http
} // namespace webserv
