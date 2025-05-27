/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:14:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 16:47:20 by adjoly           ###   ########.fr       */
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
	Client(int, config::Server *);
	Client(const Client &cpy);
	virtual ~Client(void);

	void parse(void);
	void answer(void);

	bool requestParsed(void);

	int getPollfd(void) const { return _fd; }

	bool operator==(int fd) {
		if (fd != _fd)
			return false;
		return true;
	}

	bool isReadyToClose() const;

  private:
	void _getRequest(std::string);

	std::string _sanitizeStr(std::string &str) {
		std::string newStr = str;

		if (str[str.size() - 1] == '\r') {
			newStr.erase(str.size() - 1);
		}
		return newStr;
	}

	int				_fd;
	http::ARequest *_request;
	http::Response	_response;
	config::Server *_conf;
	config::Route * _route;
	size_t			_bytes_sent;
	std::string		_response_str;
	bool			_response_done;
};

} // namespace server
} // namespace webserv
