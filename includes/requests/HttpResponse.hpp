/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:21:20 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/03 18:11:27 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __REQUESTS_HTTP_RESPONSE_HPP__
# define __REQUESTS_HTTP_RESPONSE_HPP__

#include "requests/default.hpp"

class HttpResponse {
public:
	HttpResponse(const HttpRequest	&request);
	HttpResponse(const HttpResponse	&cpy);
	~HttpResponse(void);

	HttpResponse	&operator=(const HttpResponse &cpy);

	std::string	str(void) const;

private:
	std::string	_protocol;
	size_t		_status_code;
	std::string	_status_text;
	std::map<std::string, std::string> _headers;
	std::string _body;

};

#endif
