/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:13:56 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/26 16:48:32 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>
#include <vector>

namespace webserv {

class Cgi {
public:
  /**
   *	@brief	Contructor that can be used to execute a cgi file
   *
   *	@param	The path of the cgi program
   *	@param	The location where the script will be executed
   *	@param	The name of cgi script that will be executed
   *	@param	A map of the http header
   *	@param	A string of the http method used
   */
  Cgi(std::string &, std::string &, std::string &, std::map<std::string, std::string> &,
      std::string &);
  ~Cgi(void);

  /**
   *	@brief	Can be used to get the response of the cgi
   *
   *	@return	The http response of the cgi
   */
  std::string getResponse(void);

protected:
private:
  std::string &_cgi_exec_path; ///> The path of the cgi executable
  std::string &_cgi_file_path; ///> The path of the file to be executed
  std::string &_http_methods;  ///> The http method of the request
  std::map<std::string, std::string>
      &_http_header; ///> A map of the header of the http request

  std::vector<std::string>
      __cgi_envp; ///> The envp that will be used to exec the cgi
  std::vector<std::string>
      _cgi_argv; ///> The argv that will be used to exec the cgi
				 
  /// more internal function will be added but none of your business :D
  /// only shit will be here
};

}; // namespace webserv
