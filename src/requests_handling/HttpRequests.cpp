/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:07:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/03/17 14:20:31 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/HttpRequest.hpp>

using namespace webserv;

std::string	http::IRequest::str(void) const
{
	std::ostringstream response;

	response << this->_method << " " << this->_target << " " << this->_protocol;
	response << "\r\n";

	for (std::multimap<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
		response << it->first << ": " << it->second << "\r\n";

	response << "\r\n";
	response << this->_body;

	return (response.str());
}

void			parse(std::string const &data) { (void) data; }
http::Response	execute(void) { return (http::Response()); }

std::string	http::IRequest::getMethod(void) const
{
	return (this->_method);
}

std::string	http::IRequest::getTarget(void) const
{
	return (this->_target);
}

std::string	http::IRequest::getProtocol(void) const
{
	return (this->_protocol);
}

void	http::IRequest::setMethod(std::string const method)
{
	this->_method = method;
}

void	http::IRequest::setTarget(std::string const target)
{
	this->_target = target;
}

void	http::IRequest::setProtocol(std::string const protocol)
{
	this->_protocol = protocol;
}

// ------------------------------------------------------------------

http::Get::Get(void)
{
}

http::Get::Get(std::string &data)
{
	this->parse(data);
}

void	http::Get::parse(std::string const &data)
{
	std::istringstream	stream(data);
	std::string			line;

	if (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		this->_target.insert(this->_target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r")
	{
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos)
		{
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			this->_headers.insert(std::make_pair(key, value));
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	///*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::endl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
	  std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "-- body --" << std::endl << this->_body << std::endl;
	//*/
}

http::Response	http::Get::execute(void)
{
	http::Response	response;

	try
	{
		std::ifstream file(this->_target.c_str(), std::ios::binary);
		response.setProtocol(this->_protocol);
		response.setStatusCode(200);
		response.setStatusText("OK");
		response.addHeader("Content-Type", "text/html"); // TODO: change it to check the file extension and set it to the corresponding MIME or text/plain if unkown. we will only implement the important MIME types in the Mozilla documentation because https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/MIME_types/Common_types

		std::ifstream file_end(this->_target.c_str(), std::ios::binary | std::ios::ate);
		std::stringstream	length;
		length << (file_end.tellg() - file.tellg());
		std::cout << length.str();
		response.addHeader("Content-Length", length.str());

		response.setBody(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
	}
	catch (...)
	{
		// TODO: replace with a predefined array of error pages
		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.setStatusText("Not Found");
		response.addHeader("Content-Type", "text/html");
		response.setBody("<html><body>nuh uh, get 404'd >:D</body></html>");
	}

	return (response);
}

// ------------------------------------------------------------------

http::Delete::Delete(void)
{
}

http::Delete::Delete(std::string &data)
{
	this->parse(data);
}

void	http::Delete::parse(std::string const &data)
{
	std::istringstream	stream(data);
	std::string			line;

	if (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		this->_target.insert(this->_target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r")
	{
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos)
		{
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			this->_headers.insert(std::make_pair(key, value));
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	///*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::endl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
	  std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "-- body --" << std::endl << this->_body << std::endl;
	//*/
}

http::Response	http::Delete::execute(void)
{
	http::Response	response;

	try
	{
		if (std::remove(this->_target.c_str()))
			throw;
		response.setProtocol(this->_protocol);
		response.setStatusCode(204); // this cool dude on the internet said i should not do that so i'll change it https://blog.ploeh.dk/2013/04/30/rest-lesson-learned-avoid-204-responses/
		response.setStatusText("No Content");
		time_t now = std::time(NULL);
		response.addHeader("Date", std::string(std::ctime(&now)));
	}
	catch (...)
	{
		// TODO: check errno value and get corresponding error page, check for corresponding error code : https://cdn.discordapp.com/attachments/784779058407014403/1350841524778307586/image.png?ex=67d8dd74&is=67d78bf4&hm=c030468d3862627d6402bf200960d1a15249ba2f8dac772af3283b368a77f2f5&

		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.setStatusText("Not Found");
		response.addHeader("Content-Type", "text/html");
		response.setBody("<html><body>nuh uh, get 404'd >:D</body></html>");
	}

	return (response);
}

// ------------------------------------------------------------------

http::Post::Post(void)
{
}

http::Post::Post(std::string &data)
{
	this->parse(data);
}

void	http::Post::parse(std::string const &data)
{
	std::istringstream	stream(data);
	std::string			line;

	if (std::getline(stream, line))
	{
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		this->_target.insert(this->_target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r")
	{
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos)
		{
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			this->_headers.insert(std::make_pair(key, value));
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	///*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::endl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
	  std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	std::cout << "-- body --" << std::endl << this->_body << std::endl;
	//*/
}

http::Response	http::Post::execute(void)
{
	http::Response	response;

	// uh idk anymore
	
	return (response);
}

// ------------------------------------------------------------------
