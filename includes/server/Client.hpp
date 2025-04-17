/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:14:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/17 18:48:25 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config/default.hpp"
#include "default.hpp"
#include "requests/default.hpp"
#include <netinet/in.h>
#include <webserv.hpp>

namespace webserv {
namespace server {

class Client {
  public:
	Client(int, sockaddr_in, config::Server *);
	~Client(void);

	void	answer(void);

  private:
	void	_getRequest(std::string);

	int				   _fd;
	struct sockaddr_in _client_addr;
	http::IRequest	  *_request;
	http::Response	  *_response;
	config::Server	  *_conf;
	Logger            *_log;
};

} // -namespace server
} // -namespace webserv
