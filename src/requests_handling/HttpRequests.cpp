/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:07:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/11 10:19:54 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "requests/default.hpp"
/*
HttpRequest parseRequest(const std::string &rawRequest)
{
	std::istringstream	stream(rawRequest);
	HttpRequest			request;
	std::string			line;

	if (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		line_stream >> request.method >> request.target >> request.protocol;
		request.target.insert(request.target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r")
	{
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos)
		{
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			request.headers[key] = value;
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	request.body = body_stream.str();

	std::cout << "method: " << request.method << std::endl;
	std::cout << "target: " << request.target << std::endl;
	std::cout << "protocol: " << request.protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = request.headers.begin(); it != request.headers.end(); ++it)
	  std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "-- body --" << std::endl << request.body << std::endl;

	return request;
}
*/
