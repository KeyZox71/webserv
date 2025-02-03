/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:00 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/03 18:11:29 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __REQUESTS_HTTP_REQUEST_HPP__
# define __REQUESTS_HTTP_REQUEST_HPP__

struct HttpRequest {
	std::string method;
	std::string target;
	std::string protocol;
	std::map<std::string, std::string> headers;
	std::string body;
};

HttpRequest parseRequest(const std::string &rawRequest);

#endif
