/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:14:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/25 13:20:13 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/default.hpp>
#include <netinet/in.h>
#include <requests/default.hpp>
#include <server/default.hpp>
#include <webserv.hpp>

namespace webserv {
namespace server {

class Client {
  public:
	Client();
	Client(struct pollfd *, sockaddr_in, config::Config *);
	void parse(void);
	virtual ~Client(void);

	void answer(void);

  private:
	void _getRequest(std::string);

	struct pollfd	  *_pfd;
	struct sockaddr_in _client_addr;
	http::IRequest	  *_request;
	// http::Response	  *_response;
	config::Server *_conf;
	config::Config	*_Gconf;
};

} // namespace server
} // namespace webserv
