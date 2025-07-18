/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiIn.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/07/02 11:56:36 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"
#include <log.hpp>
#include <server/PfdManager.hpp>
#include <stdexcept>
#include <sys/poll.h>
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

		_log->debug("post body : " + body);
	}
	~CgiIn(void) { log("➖", "CgiIn", "destructor called"); }

	void process(void) {
		_processed = true;
		ssize_t bytes_written = write(_fd, _body.c_str(), _body.size());
		_log->debug("writting body : " + _body);
		if (bytes_written == -1)
			throw std::runtime_error(
				"write error could not write body to cgi stdin");
		bytes_written = write(_fd, "\0", 1);
		_log->debug("writting end bytes");
		if (bytes_written == -1)
			throw std::runtime_error(
				"write error could not write end byte to cgi stdin");
		PfdManager::remove(_fd);
	}
	clientResType type(void) const { return CGI_IN; }
	short		  event(void) const { return POLLIN; }
	bool		  isReady(void) const {
		 if (PfdManager::get(_fd)->revents & event()) {
			 _log->debug("CgiIn not ready");
			 return false;
		 }
		 _log->debug("CgiIn ready");
		 return true;
	}

  protected:
  private:
	std::string _body;
};

} // namespace server
} // namespace webserv
