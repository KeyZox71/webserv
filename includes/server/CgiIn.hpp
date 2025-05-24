/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiIn.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/24 11:15:25 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"
#include <log.hpp>

namespace webserv {
namespace server {

class CgiIn : public AClientResource {
  public:
	CgiIn(std::string body, int id) : _body(body) {
		log("➕", "CgiIn", "constructor called");
		_fd->fd = id;
		_fd->events = POLLOUT;
	}
	~CgiIn(void) { log("➖", "CgiIn", "destructor called"); }

	void send(void) {
		// TODO: send the body
	}
	clientResType type(void) const { return CGI_IN; }

  protected:
  private:
	std::string _body;
};

} // namespace server
} // namespace webserv
