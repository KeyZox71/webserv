/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:43 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/29 13:17:58 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cgi.hpp>
#include <webserv.hpp>

using namespace webserv;

Cgi::Cgi(std::string &, std::string &, std::string &,
         std::map<std::string, std::string> &, std::string &) {}

void Cgi::_getCgiEnvp(void) {
  _cgi_envp.push_back("SERVER_SOFTWARE=webserv/" + std::string(WSRV_VERSION));

  std::map<std::string, std::string>::iterator it = _http_header.find("Host");
  if (it != _http_header.end())
    _cgi_envp.push_back("SERVER_NAME" + it->second);
}
