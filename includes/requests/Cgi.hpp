/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:17:34 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/25 13:40:10 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requests/HttpIMessage.hpp"
#include "requests/HttpRequest.hpp"
#include <config/default.hpp>
#include <map>
#include <string>

namespace webserv {

class Cgi {
  public:
	Cgi(http::IRequest *, config::Server *);
	~Cgi(void);

	std::string getEnv(std::string &);
	void		setEnv(std::string &, std::string);

	void process(void);

  protected:
  private:
	void _initEnvp(void);

	std::map<std::string, std::string> _envp;
	config::Server					  *_conf;
	http::IMessage					  *_request;
};
}; // namespace webserv
