/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/05/20 20:01:38 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"
#include <config/Route.hpp>
#include <config/default.hpp>
#include <cstdio>
#include <requests/ARequest.hpp>

#include <map>
#include <string>
#include <unistd.h>

namespace webserv {

class Cgi : public server::AClientResource {
  public:
	Cgi(http::ARequest *, config::Route *);
	~Cgi(void);

	int getFdOut(void) const { return _stdout_pipe[0]; }

	/**
	 *	@brief	Can be used to prepare the Cgi execution
	 *
	 *	@note	To be used after the main fd is in POLLOUT state
	 */
	void prepare(void);

	/**
	 *	@brief	Can be used to know if the prepare function has already been
	 *			called
	 */
	bool isPrepared(void) { return _prepared; }

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
	bool _prepared;
	bool _executed;

	void _initEnvp(void);

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
	config::Route  *_conf;	  // The configuration for the route used
	http::ARequest *_request; // The requests that will be used for the cgi

	int _stdin_pipe[2];	 // The pipefd for the stdin of the cgi
	int _stdout_pipe[2]; // The pipefd for the stdout of the cgi
};

}; // namespace webserv
