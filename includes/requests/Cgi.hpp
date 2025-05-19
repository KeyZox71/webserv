/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/19 11:07:07 by adjoly           ###   ########.fr       */
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

	/**
	 *	@brief	Can be used to convert the _envp to a char** usable in execve
	 *
	 *	@return	A newly allocated char** with the env from _envp
	 */
	char **_genEnv(void);

	std::string _script_path; // The full path of the script to be executed
	std::string	_cgi_path;

	std::map<std::string, std::string> _envp; // The envp filled with _initEnvp
	config::Route  *_conf; // The configuration for the route used
	http::ARequest *_request; // The requests that will be used for the cgi

	int	stdin_pipe[2]; // The pipefd for the stdin of the cgi
	int stdout_pipe[2]; // The pipefd for the stdout of the cgi
};

}; // namespace webserv
