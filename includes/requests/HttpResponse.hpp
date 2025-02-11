/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:21:20 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/08 05:06:57 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __REQUESTS_HTTP_RESPONSE_HPP__
# define __REQUESTS_HTTP_RESPONSE_HPP__

#include "requests/default.hpp"

class IHttpResponse {
public:
	void	parseRequest(const HttpRequest &request) = 0;

	std::string	str(void) const;

protected:
	std::string	_protocol;
	size_t		_status_code;
	std::string	_status_text;
	std::map<std::string, std::string> _headers;
	std::string _body;

};

class HttpGet: public IHttpResponse {
public:
	HttpGet(void);
	HttpGet(const HttpRequest	&request);
	~HttpResponse(void);

	void	parseRequest(const HttpRequest &request);
}

#endif
