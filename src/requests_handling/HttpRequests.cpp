/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequests.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 16:07:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/28 14:32:33 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <algorithm>

#include <requests/HttpRequest.hpp>
#include <requests/Errors.hpp>
#include <sys/stat.h>
#include <dirent.h>

#include <log.hpp>

using namespace webserv;

http::IRequest::~IRequest(void) {

}

std::string	http::IRequest::str(void) const
{
	std::ostringstream response;

	response << this->_method << " " << this->_target << " " << this->_protocol;
	response << "\r\n";

	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
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

http::Get::~Get(void)
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

	/*
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
	*/
}

char	isDirectory(const std::string& path)
{
	struct stat file_stat;

	if (stat(path.c_str(), &file_stat) != 0)
	{
		throw std::runtime_error("can't open file (non-existant ?)");
	}
	return S_ISDIR(file_stat.st_mode);
}

http::Response	http::Get::execute(void)
{
	http::Response	response;

	try
	{
		if (isDirectory(this->_target))
		{
			DIR *dir;
			struct dirent *entry;
			struct stat file_stat;
			std::vector<std::string> files;
			
			if ((dir = opendir(this->_target.c_str())) == NULL)
				throw;
			while ((entry = readdir(dir)) != NULL)
			{
				std::string file_name = entry->d_name;
				if (file_name == ".")
					continue;
				std::string file_path = this->_target + "/" + file_name;
				if (stat(file_path.c_str(), &file_stat) == 0)
				{
					if (S_ISDIR(file_stat.st_mode))
						files.push_back(file_name + "/");
					else
						files.push_back(file_name);
				}
			}
			closedir(dir);

			std::sort(files.begin(), files.end());

			std::string body = "<html>";

			body += "<head><style>\n\
:root {\n\
  background-color: -moz-dialog;\n\
  color: -moz-dialogtext;\n\
  font: message-box;\n\
  padding-inline: 2em;\n\
  color-scheme: light dark;\n\
}\n\
\n\
body {\n\
  border: 1px solid ThreeDShadow;\n\
  border-radius: 10px;\n\
  padding: 3em;\n\
  min-width: 30em;\n\
  max-width: 65em;\n\
  margin: 4em auto;\n\
  background-color: Field;\n\
  color: FieldText;\n\
}\n\
</style></head>";

			body += "<body><ul>\n";
			for (size_t i = 0; i < files.size(); i++)
				body += "<li><a href=\"" + files[i] + "\">" + files[i] + "</a></li>\n";
			body += "</ul></body></html>";

			response.setProtocol(this->_protocol);
			response.setStatusCode(200);
			std::stringstream	length;
			length << body.length();
			response.addHeader("Content-Length", length.str());
			response.addHeader("Content-Type", "text/html");
			response.setBody(body);
			
		}
		else
		{
			std::ifstream file(this->_target.c_str(), std::ios::binary);
			std::streampos file_start = file.tellg();
			response.setBody(std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()));
			std::stringstream	length;
			length << (file.tellg() - file_start);
			response.addHeader("Content-Length", length.str());

			response.setProtocol(this->_protocol);
			response.setStatusCode(200);
			response.addHeader("Content-Type", http::Mime::getType(this->_target));

#ifdef VERBOSE
			//_log->debug(response.str().c_str());
#endif
		}
	}
	catch (...)
	{
		// TODO: replace with a predefined array of error pages
		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(response.getStatusCode()));
	}

	return (response);
}

// ------------------------------------------------------------------

http::Delete::Delete(void)
{
}

http::Delete::~Delete(void)
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

	/*
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
	*/
}

http::Response	http::Delete::execute(void)
{
	http::Response	response;

	try
	{
		if (std::remove(this->_target.c_str()))
			throw std::runtime_error("can't remove file, FF");
		response.setProtocol(this->_protocol);
		response.setStatusCode(204);
		time_t now = std::time(NULL);
		response.addHeader("Date", std::string(std::ctime(&now)));
	}
	catch (...)
	{
		// TODO: check errno value and get corresponding error page, check for corresponding error code : https://cdn.discordapp.com/attachments/784779058407014403/1350841524778307586/image.png?ex=67d8dd74&is=67d78bf4&hm=c030468d3862627d6402bf200960d1a15249ba2f8dac772af3283b368a77f2f5&

		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(response.getStatusCode()));
	}

	return (response);
}

// ------------------------------------------------------------------

http::Post::Post(void)
{
}

http::Post::~Post(void)
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

	/*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::endl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); ++it)
	  std::cout << it->first << ": " << it->second << std::endl;
	std::cout << std::endl;
	//std::cout << "-- body --" << std::endl << this->_body << std::endl;
	*/
}

std::string extractFilename(const std::string &header)
{
	size_t start = header.find("filename=\"") + 10;
	size_t end = header.find("\"", start);
	return header.substr(start, end - start);
}

void handleMultipartData(const std::string &body, const std::string &boundary)
{
	size_t		i = 0;
	std::string	delim = "--" + boundary;
	delim.erase(delim.size() - 1);

	while ((i = body.find(delim, i)) != std::string::npos)
	{
		size_t	start = i + delim.length();
		size_t end = body.find("\r\n\r\n", start);

		if (end != std::string::npos)
		{
			std::string part_header = body.substr(start, end - start);
			//std::cout << std::endl << std::endl << std::endl << std::endl;
			std::string part_content = body.substr(end + 4, body.find(delim, end) - end - 4);

			std::ofstream outfile(extractFilename(part_header).c_str(), std::ios::binary);
			if (outfile.is_open())
			{
				outfile.write(part_content.c_str(), part_content.length());
				outfile.close();
			}
			else
			{
				std::cerr << "open failed" << std::endl;
			}
		}

		i += delim.length();
	}
}

http::Response	http::Post::execute(void)
{
	http::Response	response;

	try
	{
		handleMultipartData(this->_body, this->getHeaders()["Content-Type"].substr(this->getHeaders()["Content-Type"].find("=", this->getHeaders()["Content-Type"].find(";")) + 1));

		response.setProtocol(this->_protocol);
		response.setStatusCode(200);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(response.getStatusCode()));
	}
	catch (...)
	{
		response.setProtocol(this->_protocol);
		response.setStatusCode(500);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(response.getStatusCode()));
	}
	return (response);
}

// ------------------------------------------------------------------
