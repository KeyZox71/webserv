/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gadelbes <gadelbes@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:46:34 by gadelbes          #+#    #+#             */
/*   Updated: 2025/07/15 20:27:43 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "log.hpp"
#include "server/PfdManager.hpp"
#include <config/default.hpp>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <server/AResource.hpp>

#include <exception>
#include <map>
#include <server/CgiIn.hpp>
#include <server/ResourceManager.hpp>
#include <sstream>
#include <string>
#include <unistd.h>
#include <wait.h>

namespace webserv {
namespace http {
class Get;
class Post;
class ARequest;
}; // namespace http
namespace server {

#define PIPE_READ 0
#define PIPE_WRITE 1

class Cgi : public server::AClientResource {
  public:
	Cgi(webserv::http::Get *, webserv::config::Route *);
	Cgi(webserv::http::Post *, webserv::config::Route *);
	~Cgi(void);

	clientResType type(void) const { return CGI; }

	/**
	 *	@brief	Can be used to process the Cgi script
	 */
	void process(void);

	/**
	 *	@brief	Can be used to get the output of the Cgi
	 *
	 *	@note	Need to be used after the outfd is in POLLIN state
	 */
	std::string str(void);

	short event(void) const { return POLLIN; }
	bool  isReady(void) const {
		 if (_is_post == false) {
			 _log->debug("CGIIII not a post");
			 return true;
		 }
		 if (static_cast<CgiIn *>(ResourceManager::get(_stdin_pipe[PIPE_WRITE]))
				 ->isProcessed() == true) {
			 _log->debug("CGIIII post readyyy");
			 return true;
		 }
		 return false;
	}

	bool isTimedout(void) {
		if (!isProcessed() && isReady()) {
			return false;
		}
		if (std::difftime(std::time(NULL), _start_time) >= 10) {
			kill(_forkPid, SIGKILL);
			waitpid(_forkPid, NULL, 0);
			_log->warn("Cgi close due to timeout >= 10s");
			return true;
		}
		return false;
	}

	bool isErr(void) {
		if (_err == true) {
			return true;
		}
		if (_forkPid == -1) {
			return false;
		}

		int	  status = 0;
		pid_t result = waitpid(_forkPid, &status, WNOHANG);

		if (result == 0 ) {
			return false;
		} else if (result == -1) {
			if (errno == ECHILD) {
				_log->warn("CGI child already reaped");
				_err = true;
				return true;
			}
			_log->error("waitpid failed: " + std::string(std::strerror(errno)));
			_err = true;
			return true;
		}

		if (WIFEXITED(status)) {
			int code = WEXITSTATUS(status);
			if (code != 0) {
				std::stringstream str;
				str << "CGI exited with status code " << code;
				_log->error(str.str());
				_err = true;
				return true;
			}
			return false;
		}

		if (WIFSIGNALED(status)) {
			std::stringstream str;
			str << "CGI terminated by signal: ";
			str << WTERMSIG(status);
			_log->error(str.str());
			_err = true;
			return true;
		}

		_log->error("CGI ended abnormally");
		_err = true;
		return true;
	}

  protected:
  private:
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

	std::map<std::string, std::string> _envp; // The envp filled with _initEnvp
	webserv::config::Route *_conf; // The configuration for the route used
	http::ARequest *_request;	   // The requests that will be used for the cgi

	int _stdin_pipe[2];	 // The pipefd for the stdin of the cgi in the case of a
						 // post
	int _stdout_pipe[2]; // The pipefd for the stdout of the cgi

	std::time_t _start_time; // To use for timeout
	pid_t		_forkPid;	 // Can be used to kill the process
	bool		_err;
};

}; // namespace server
}; // namespace webserv
