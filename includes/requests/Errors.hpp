/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:51:46 by mmoussou          #+#    #+#             */
/*   Updated: 2025/03/24 15:05:53 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __WEBSERV_REQUESTS_ERRORS_HPP__
# define __WEBSERV_REQUESTS_ERRORS_HPP__

#include <map>
#include <string>

#include <requests/HttpResponse.hpp>

namespace webserv {
namespace http {

/*
	* DOES NOT WORK
	* still need to do uh things but base is done at least :D
*/

class Errors {
public:
	//static http::Response	&getRequest(int error_code);
	static std::string	getResponseBody(int error_code);
	static void			setEntry(const std::string &key, int value);

	static std::map<int, std::string>	message;
private:
	static std::map<int, std::string>	populateMessages();

	static std::map<int, std::string>	set_error_pages;

};

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_ERRORS_HPP__
