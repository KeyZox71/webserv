/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:14:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/30 14:51:39 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/default.hpp>
#include <cstdio>
#include <netinet/in.h>
#include <requests/default.hpp>
#include <server/Server.hpp>
#include <webserv.hpp>

namespace webserv {
namespace server {

class Client {
  public:
	Client(struct pollfd *, sockaddr_in, config::Config *);
	Client(const Client &cpy);
	virtual ~Client(void);

	void parse(void);
	void answer(void);

	bool requestParsed(void);

	struct pollfd *getPollfd(void) const { return _pfd; }

	bool operator==(int fd) {
		if (fd != _pfd->fd)
			return false;
		return true;
	}

  private:
	void _getRequest(std::string);

	std::string _sanitizeStr(std::string &str) {
		std::string	newStr = str;

		if (str[str.size() - 1] == '\r') {
			newStr.erase(str.size() - 1);
		}
		return newStr;
	}

	struct pollfd	  *_pfd;
	struct sockaddr_in _client_addr;
	http::ARequest	  *_request;
	// http::Response	  *_response;
	config::Server *_conf;
	config::Route  *_route;
	config::Config *_Gconf;
};

} // namespace server
} // namespace webserv
