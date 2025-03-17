/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:51:46 by mmoussou          #+#    #+#             */
/*   Updated: 2025/03/17 14:11:44 by mmoussou         ###   ########.fr       */
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
	static http::Response	&getRequest(int error_code);
	static void				setEntry(const std::string &key, int value);

private:
	static std::map<int, std::string>	populateMessages();

	static std::map<int, std::string>	message;
	static std::map<int, std::string>	set_error_pages;

};

/*std::map<int, std::string>	Errors::populateMessages()
{
	std::map<int, std::string> m;

	m[100] = "Continue";
	m[101] = "Switching Protocols";
	m[102] = "Processing";
	m[103] = "Early Hints";
	m[200] = "OK";
	m[201] = "Created";
	m[202] = "Accepted";
	m[203] = "Non-Authoritative Information";
	m[204] = "No Content";
	m[205] = "Reset Content";
	m[206] = "Partial Content";
	m[207] = "Multi-Status";
	m[208] = "Already Reported";
	m[226] = "IM Used";
	m[300] = "Multiple Choices";
	m[301] = "Moved Permanently";
	m[302] = "Found";
	m[303] = "See Other";
	m[304] = "Not Modified";
	m[305] = "Use Proxy";
	m[306] = "Switch Proxy";
	m[307] = "Temporary Redirect";
	m[308] = "Permanent Redirect";
	m[400] = "Bad Request";
	m[401] = "Unauthorized";
	m[402] = "Payment Required";
	m[403] = "Forbidden";
	m[404] = "Not Found";
	m[405] = "Method Not Allowed";
	m[406] = "Not Acceptable";
	m[407] = "Proxy Authentication Required";
	m[408] = "Request Timeout";
	m[409] = "Conflict";
	m[410] = "Gone";
	m[411] = "Length Required";
	m[412] = "Precondition Failed";
	m[413] = "Payload Too Large";
	m[414] = "URI Too Long";
	m[415] = "Unsupported Media Type";
	m[416] = "Range Not Satisfiable";
	m[417] = "Expectation Failed";
	m[418] = "I'm a teapot";
	m[420] = "Method Failure";
	m[421] = "Misdirected Request";
	m[422] = "Unprocessable Entity";
	m[423] = "Locked";
	m[424] = "Failed Dependency";
	m[426] = "Upgrade Required";
	m[428] = "Precondition Required";
	m[429] = "Too Many Requests";
	m[431] = "Request Header Fields Too Large";
	m[451] = "Unavailable For Legal Reasons";
	m[500] = "Internal Server error";
	m[501] = "Not Implemented";
	m[502] = "Bad Gateway";
	m[503] = "Service Unavailable";
	m[504] = "gateway Timeout";
	m[505] = "Http version not supported";
	m[506] = "Varient Also negotiate";
	m[507] = "Insufficient Storage";
	m[508] = "Loop Detected";
	m[510] = "Not Extended";
	m[511] = "Network Authentication Required";

	return m;
}

std::map<int, std::string>	Errors::message = Errors::populateMessages();

std::map<int, std::string>	Errors::set_error_pages;

http::Response	&Errors::getRequest(int error_code)
{
	http::Response	*result = new http::Response;

	if (Errors::set_error_pages.find(error_code) != Errors::set_error_pages.end())
		result->setBody(Errors::set_error_pages[error_code]);
	else
		result->setBody("<html><body><h1>" + Errors::message[error_code] + "</h1></body></html>");
	return (*result);
}*/

} // -namespace http
} // -namespace webserv

#endif // __WEBSERV_REQUESTS_ERRORS_HPP__
