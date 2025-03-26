/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 16:27:43 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/26 20:46:01 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cgi.hpp>
#include <webserv.hpp>

using namespace webserv;

Cgi::Cgi(std::string &, std::string &,
                  std::map<std::string, std::string> &, std::string &) {}

void	Cgi::_getCgiEnvp(void) {
	// Server side report
	_cgi_envp.push_back("SERVER_SOFTWARE=Webserv/" + std::string(WSRV_VERSION));
	_cgi_envp.push_back("SERVER_NAME=" + _http_header.at("Host"));
	_cgi_envp.push_back("GATEWAY_INTERFACE=CGI/1.1");

	// Request specific variable
	_cgi_envp
}
