/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiIn.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 13:08:08 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"
#include <log.hpp>
#include <unistd.h>

namespace webserv {
namespace server {

class CgiIn : public AClientResource {
  public:
	CgiIn(std::string body, int id) : _body(body) {
		log("➕", "CgiIn", "constructor called");
		_processed = false;
		_fd = id;
		_pfd_event = POLLOUT;
	}
	~CgiIn(void) {
		log("➖", "CgiIn", "destructor called");
		close(_fd);
	}

	void process(void) {
		_processed = true;
		// TODO: send the body
	}
	clientResType type(void) const { return CGI_IN; }

	short event(void) const { return POLLIN; }

  protected:
  private:
	std::string _body;
};

} // namespace server
} // namespace webserv
