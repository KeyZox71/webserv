/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 14:14:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/14 15:00:41 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "config/default.hpp"
#include "default.hpp"
#include "requests/default.hpp"
#include <netinet/in.h>
#include <webserv.hpp>

class Client {
  public:
	Client(int, sockaddr_in);
	~Client(void);

	void	answer(void);
  private:
	int				   _fd;
	struct sockaddr_in _client_addr;
	http::IRequest	  *_request;
	http::Response	  *_response;
	config::Config	  *_conf;
};
