/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/24 11:18:49 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/Route.hpp>
#include <config/default.hpp>
#include <cstdio>
#include <requests/default.hpp>
#include <server/AResource.hpp>

#include <map>
#include <string>
#include <unistd.h>

namespace webserv {
namespace server {

#define PIPE_READ 0
#define PIPE_WRITE 1

class Cgi : public server::AClientResource {
  public:
	Cgi(webserv::http::Get *, webserv::config::Route *);
	Cgi(webserv::http::Post *, webserv::config::Route *);
	~Cgi(void);

	/**
	 *	@brief	Can be used to process the Cgi script
	 */
	void process(void);

	/**
	 *	@brief	Can be used to check if the Cgi have been executed
	 *
	 *	@note	Need to be used after the process() function and checked before
	 *			using str()
	 */
	bool isProcessed(void) { return _executed; }

	/**
	 *	@brief	Can be used to get the output of the Cgi
	 *
	 *	@note	Need to be used after the outfd is in POLLIN state
	 */
	std::string str(void);

  protected:
  private:
	bool _executed;
	bool _is_post;

	void _initEnvp(void);

	void _prep(void);

	/**
	 *	@brief	Can be used to convert the _envp to a char** usable in execve
	 *
	 *	@return	A newly allocated char** with the env from _envp
	 */
	char **_genEnv(void);

	std::string _getEnv(std::string &) const;
	void		_setEnv(const std::string, std::string);

	std::string _script_path; // The full path of the script to be executed
	std::string _cgi_path;

	std::map<std::string, std::string> _envp; // The envp filled with _initEnvp
	webserv::config::Route *_conf; // The configuration for the route used
	webserv::http::ARequest
		*_request; // The requests that will be used for the cgi

	int _stdin_pipe[2];	 // The pipefd for the stdin of the cgi in the case of a
						 // post
	int _stdout_pipe[2]; // The pipefd for the stdout of the cgi
};

}; // namespace server
}; // namespace webserv
