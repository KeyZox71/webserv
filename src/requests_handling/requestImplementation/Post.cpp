/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:50:20 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/30 15:18:01 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

#include <config/URL.hpp>
#include <requests/default.hpp>
#include <log.hpp>

using namespace webserv::http;

Post::Post(std::string &data) { this->parse(data); }

void Post::parse(std::string const &data) {
	std::istringstream stream(data);
	std::string		   line;

	if (std::getline(stream, line)) {
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		_method = _sanitizeStr(_method);
		_target = _sanitizeStr(_target);
		_protocol = _sanitizeStr(_protocol);
		// this->_target.insert(this->_target.begin(), '.');
	}

	while (std::getline(stream, line) && line != "\r") {
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos) {
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			this->_headers.insert(std::make_pair(key, _sanitizeStr(value)));
		}
	}

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	_url = new URL(_target);
	std::cout << *_url << std::endl;

	/*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::endl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it =
	this->_headers.begin(); it != this->_headers.end(); ++it) std::cout <<
	it->first << ": " << it->second << std::endl; std::cout << std::endl;
	//std::cout << "-- body --" << std::endl << this->_body << std::endl;
	*/
}

std::string extractFilename(const std::string &header) {
	size_t start = header.find("filename=\"") + 10;
	size_t end = header.find("\"", start);
	return header.substr(start, end - start);
}

void handleMultipartData(const std::string &body, const std::string &boundary) {
	size_t		i = 0;
	std::string delim = "--" + boundary;
	delim.erase(delim.size() - 1);

	while ((i = body.find(delim, i)) != std::string::npos) {
		size_t start = i + delim.length();
		size_t end = body.find("\r\n\r\n", start);

		if (end != std::string::npos) {
			std::string part_header = body.substr(start, end - start);
			// std::cout << std::endl << std::endl << std::endl << std::endl;
			std::string part_content =
				body.substr(end + 4, body.find(delim, end) - end - 4);

			std::ofstream outfile(extractFilename(part_header).c_str(),
								  std::ios::binary);
			if (outfile.is_open()) {
				outfile.write(part_content.c_str(), part_content.length());
				outfile.close();
			} else {
				std::cerr << "open failed" << std::endl;
			}
		}

		i += delim.length();
	}
}

Response Post::execute(void) {
	http::Response response;

	try {
		handleMultipartData(
			this->_body,
			this->getHeaders()["Content-Type"].substr(
				this->getHeaders()["Content-Type"].find(
					"=", this->getHeaders()["Content-Type"].find(";")) +
				1));

		response.setProtocol(this->_protocol);
		response.setStatusCode(200);
		response.addHeader("Content-Type", "text/html");
		response.setBody(
			http::Errors::getResponseBody(response.getStatusCode()));
	} catch (...) {
		response.setProtocol(this->_protocol);
		response.setStatusCode(500);
		response.addHeader("Content-Type", "text/html");
		response.setBody(
			http::Errors::getResponseBody(response.getStatusCode()));
	}
	return (response);
}

