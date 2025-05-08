/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/06 19:34:11 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <requests/ARequest.hpp>
#include <config/default.hpp>
#include <map>
#include <string>

namespace webserv {

class Cgi {
  public:
	Cgi(http::ARequest *, config::Server *);
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
