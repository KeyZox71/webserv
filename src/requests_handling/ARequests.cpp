/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ARequests.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:07:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/30 15:21:54 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

#include <config/URL.hpp>
#include <log.hpp>
#include <requests/default.hpp>

using namespace webserv::http;

std::string ARequest::str(void) const {
	std::ostringstream response;

	response << this->_method << " " << this->_target << " " << this->_protocol;
	response << "\r\n";

	for (std::map<std::string, std::string>::const_iterator it =
			 this->_headers.begin();
		 it != this->_headers.end(); ++it)
		response << it->first << ": " << it->second << "\r\n";

	response << "\r\n";
	response << this->_body;

	return (response.str());
}

void	 parse(std::string const &data) { (void)data; }
Response execute(void) { return (Response()); }

std::string ARequest::getMethod(void) const { return (this->_method); }

std::string ARequest::getTarget(void) const { return (this->_target); }

std::string ARequest::getProtocol(void) const { return (this->_protocol); }

void ARequest::setMethod(std::string const method) { this->_method = method; }

void ARequest::setTarget(std::string const target) { this->_target = target; }

void ARequest::setProtocol(std::string const protocol) {
	this->_protocol = protocol;
}

URL		ARequest::getUrl() const
{
	if (this->_url)
		return *(this->_url);
	else
		return URL("");
}
