/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:50:20 by adjoly            #+#    #+#             */
/*   Updated: 2025/07/15 19:17:08 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

#include <config/URL.hpp>
#include <log.hpp>
#include <requests/default.hpp>
#include <server/default.hpp>

using namespace webserv::http;

Response parseCgiOut(std::string cgi_str);

Post::Post(std::string &data, config::Server *srv) {
	_url = not_nullptr;
	_srv = srv;
	_cgi = not_nullptr;
	this->parse(data);
}

static std::string urlDecode(const std::string& str) {
    std::ostringstream decoded;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            std::string hex = str.substr(i + 1, 2);
            char decodedChar = static_cast<char>(std::strtol(hex.c_str(), 0, 16));
            decoded << decodedChar;
            i += 2;
        } else {
            decoded << str[i];
        }
    }
    return decoded.str();
}

void Post::parse(std::string const &data) {
	size_t header_end = data.find("\r\n\r\n");
	if (header_end == std::string::npos)
		throw std::runtime_error("Invalid HTTP request");
	this->_body = data.substr(header_end + 4);

	std::istringstream stream(data.substr(0, header_end));
	std::string		   line;

	if (std::getline(stream, line)) {
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		_method = _sanitizeStr(_method);
		_target = urlDecode(_sanitizeStr(_target));
		_protocol = _sanitizeStr(_protocol);
	}

	while (std::getline(stream, line) && line != "\r") {
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos) {
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			this->_headers.insert(std::make_pair(key, _sanitizeStr(value)));
		}
	}

	_route = _srv->whatRoute(URL(_target));
	/*if (_route->getMaxBody() != -1 &&
		(int32_t)_body.length() > _route->getMaxBody()) {
		_method = "413";
		_log->warn("post body too large");
		return;
	}*/

	_url = new URL(_target);

	std::string targ = _target;

	if (targ[targ.length() - 1] == '/') {
		targ += _route->getIndex();
	}

	if (_route->isCgi(targ)) {
		_log->debug("cgi added");
		try {
			_cgi = new server::Cgi(this, _route);
		} catch (std::exception &e) {
			_log->error(e.what());
			_method = "500";
			return;
		}
		server::ResourceManager::append(_cgi);
		struct pollfd pfd;
		pfd.events = _cgi->event();
		pfd.revents = 0;
		pfd.fd = _cgi->getId();
		server::PfdManager::append(pfd, server::RES);
	}
}

std::string Post::extractFilename(const std::string &header) {
	size_t start = header.find("filename=\"") + 10;
	size_t end = header.find("\"", start);
	return this->_route->getUpRoot() + header.substr(start, end - start);
}

void Post::handleMultipartData(const std::string &body,
							   const std::string &boundary) {
	_log->debug("handling MultipartData upload...");
	size_t		i = 0;
	std::string delim = "--" + boundary;
	delim.erase(delim.size() - 1);

	while ((i = body.find(delim, i)) != std::string::npos) {
		size_t start = i + delim.length();
		size_t end = body.find("\r\n\r\n", start);

		if (end != std::string::npos) {
			std::string part_header = body.substr(start, end - start);
			std::string part_content =
				body.substr(end + 4, body.find(delim, end) - end - 4);

			std::ofstream outfile(
				(this->_route->getUpRoot() + extractFilename(part_header))
					.c_str(),
				std::ios::binary);
			if (outfile.is_open()) {
				outfile.write(part_content.c_str(), part_content.length());
				outfile.close();
			} else {
				_log->error("open failed D:");
				throw std::runtime_error("open failed");
			}
		}

		i += delim.length();
	}
}

void Post::handleBinaryUpload() {
	_log->debug("handling binary upload...");
	std::cout << (this->_route->getUpRoot() + this->_target) << std::endl;
	std::ofstream outfile((this->_route->getUpRoot() + this->_target).c_str(),
						  std::ios::binary);

	if (outfile.is_open()) {
		outfile.write(this->_body.data(), this->_body.length());
	} else {
		_log->error("open failed D:");
		outfile.close();
		throw std::runtime_error("open failed");
	}
	outfile.close();
}

Response Post::execute(void) {
	http::Response response;

	if (_cgi != not_nullptr) {
		if (_method == "500" || _cgi->isTimedout()) {
			response.setStatusCode(500);
			response.setProtocol(_protocol);
			response.addHeader("Content-Type", "text/html");
			response.setBody(http::Errors::getResponseBody(
				response.getStatusCode(),
				_srv->getErrorPage(response.getStatusCode())));
			server::PfdManager::remove(_cgi->getId());
			server::ResourceManager::remove(_cgi->getId());
			_cgi = not_nullptr;
			return response;
		}
		std::string str = static_cast<server::Cgi *>(_cgi)->str();
		response = parseCgiOut(str);
		response.setProtocol(_protocol);
		server::PfdManager::remove(_cgi->getId());
		server::ResourceManager::remove(_cgi->getId());
		_cgi = not_nullptr;
		return response;
	}

	try {
		if (this->_route->getUpRoot().empty()) {
			_log->error("invalid upload path");
			throw std::runtime_error("invalid upload path");
		}

		if (this->getHeaders()["Content-Type"].substr(0, 19) ==
			"multipart/form-data")
			handleMultipartData(
				this->_body,
				this->getHeaders()["Content-Type"].substr(
					this->getHeaders()["Content-Type"].find(
						"=", this->getHeaders()["Content-Type"].find(";")) +
					1));
		else
			handleBinaryUpload();

		response.setProtocol(this->_protocol);
		response.setStatusCode(200);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(
			response.getStatusCode(),
			_srv->getErrorPage(response.getStatusCode())));
	} catch (...) {
		response.setProtocol(this->_protocol);
		response.setStatusCode(500);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(
			response.getStatusCode(),
			_srv->getErrorPage(response.getStatusCode())));
	}
	return (response);
}

Post::~Post(void) {
	if (_url != not_nullptr)
		delete _url;
}
