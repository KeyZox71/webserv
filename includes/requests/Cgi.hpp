/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/15 13:45:20 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"
#include <config/Route.hpp>
#include <config/default.hpp>
#include <requests/ARequest.hpp>

#include <map>
#include <string>

namespace webserv {

class Cgi : public server::AClientResource {
  public:
	Cgi(http::ARequest *, config::Route *);
	~Cgi(void);

	std::string getEnv(std::string &);
	void		setEnv(const std::string, std::string);

	void process(void);

  protected:
  private:
	void _initEnvp(void);

	char **_genEnv(void);

	std::map<std::string, std::string> _envp;
	config::Route					  *_conf;
	http::ARequest					  *_request;
};

}; // namespace webserv
