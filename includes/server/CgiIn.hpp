/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiIn.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/30 16:16:28 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"
#include <log.hpp>
#include <stdexcept>
#include <unistd.h>
#include <server/PfdManager.hpp>

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
		std::cout << "processing cginin" << std::endl;
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
	bool		  isReady(void) const { return true; }

  protected:
  private:
	std::string _body;
};

} // namespace server
} // namespace webserv
