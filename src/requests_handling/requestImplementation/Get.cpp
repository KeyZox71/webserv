/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:40:16 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/30 16:39:08 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "requests/Response.hpp"
#include <algorithm>
#include <cstddef>
#include <dirent.h>
#include <exception>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

#include <requests/default.hpp>
#include <server/default.hpp>

using namespace webserv::http;

Get::Get(std::string &data, config::Server *srv) {
	_cgi = not_nullptr;
	_srv = srv;
	_url = not_nullptr;
	this->parse(data);
}

Get::~Get(void) {
	if (_url != not_nullptr) 
		delete _url;
}

void Get::parse(std::string const &data) {
	std::istringstream stream(data);
	std::string		   line;

	if (std::getline(stream, line)) {
		std::istringstream line_stream(line);
		line_stream >> this->_method >> this->_target >> this->_protocol;
		_method = _sanitizeStr(_method);
		_target = _sanitizeStr(_target);
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

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	_url = new URL("http://" + _headers["Host"] + _target);

	std::string targ = _target;

	if (targ[targ.length() - 1] == '/') {
		targ += _route->getIndex();
	}

	if (_route->isCgi(targ)) {
		_log->info("cgi added");
		try {
			_cgi = new server::Cgi(this, _route);
		} catch (std::exception &e) {
			_log->error(e.what());
			_method = "500";
			delete _url;
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

char isDirectory(const std::string &path) {
	struct stat file_stat;

	if (stat(path.c_str(), &file_stat) != 0) {
		throw std::runtime_error("can't open file (non-existant ?)");
	}
	return S_ISDIR(file_stat.st_mode);
}

Response parseCgiOut(std::string cgi_str) {
	Response		   response;
	std::istringstream stream(cgi_str);
	std::string		   line;

	response.setStatusCode(200);
	while (std::getline(stream, line) && line != "") {
		size_t delimiter_index = line.find(':');
		if (delimiter_index != std::string::npos) {
			std::string key = line.substr(0, delimiter_index);
			std::string value = line.substr(delimiter_index + 2);
			response.addHeader(key, value);
		}
	}
	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	response.setBody(body_stream.str());

	if (response.getHeader("Content-Length") == "") {
		std::stringstream length;
		length << response.getBody().length();
		response.addHeader("Content-Length", length.str());
	}
	return response;
}

Response Get::execute(void) {
	http::Response response;

	if (_cgi != not_nullptr) {
		if (_method == "500") {
			response.setStatusCode(500);
			response.addHeader("Content-Type", "text/html");
			response.setBody(http::Errors::getResponseBody(
				response.getStatusCode(),
				_srv->getErrorPage(response.getStatusCode())));
			server::PfdManager::remove(_cgi->getId());
			server::ResourceManager::remove(_cgi->getId());
			_cgi = not_nullptr;
			// if (_url != not_nullptr)
			// 	delete _url;
			return response;
		}
		std::string str = static_cast<server::Cgi *>(_cgi)->str();
		response = parseCgiOut(str);
		response.setProtocol(_protocol);
		server::PfdManager::remove(_cgi->getId());
		server::ResourceManager::remove(_cgi->getId());
		_cgi = not_nullptr;
		// if (_url != not_nullptr)
		// 	delete _url;
		return response;
	}

	this->_target = this->_route->getRootDir() + this->_target;

	try {
		if (isDirectory(this->_target)) {
			if (!access((this->_target + this->_route->getIndex()).c_str(),
						R_OK)) {
				this->_target = this->_target + this->_route->getIndex();

				std::ifstream  file(this->_target.c_str(), std::ios::binary);
				std::streampos file_start = file.tellg();
				response.setBody(
					std::string((std::istreambuf_iterator<char>(file)),
								std::istreambuf_iterator<char>()));
				std::stringstream length;
				length << (file.tellg() - file_start);
				response.addHeader("Content-Length", length.str());

				response.setProtocol(this->_protocol);
				response.setStatusCode(200);
				response.addHeader("Content-Type",
								   http::Mime::getType(this->_target));
			} else if (this->_route->getDirList()) {
				DIR *					 dir;
				struct dirent *			 entry;
				struct stat				 file_stat;
				std::vector<std::string> files;

				if ((dir = opendir(this->_target.c_str())) == NULL) {
					// if (_url != not_nullptr)
					// 	delete _url;
					throw;
				}
				while ((entry = readdir(dir)) != NULL) {
					std::string file_name = entry->d_name;
					if (file_name == ".")
						continue;
					std::string file_path = this->_target + "/" + file_name;
					if (stat(file_path.c_str(), &file_stat) == 0) {
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
					body += "<li><a href=\"" + files[i] + "\">" + files[i] +
							"</a></li>\n";
				body += "</ul></body></html>";

				response.setProtocol(this->_protocol);
				response.setStatusCode(200);
				std::stringstream length;
				length << body.length();
				response.addHeader("Content-Length", length.str());
				response.addHeader("Content-Type", "text/html");
				response.setBody(body);
			} else {
				// if (_url != not_nullptr)
				// 	delete _url;
				throw std::runtime_error("dir but no dirlist");
			}
		} else {
			std::ifstream  file(this->_target.c_str(), std::ios::binary);
			std::streampos file_start = file.tellg();
			response.setBody(std::string((std::istreambuf_iterator<char>(file)),
										 std::istreambuf_iterator<char>()));
			std::stringstream length;
			length << (file.tellg() - file_start);
			response.addHeader("Content-Length", length.str());

			response.setProtocol(this->_protocol);
			response.setStatusCode(200);
			response.addHeader("Content-Type",
							   http::Mime::getType(this->_target));
		}
	} catch (...) {
		// TODO: replace with a predefined array of error pages
		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.addHeader("Content-Type", "text/html");
		response.setBody(http::Errors::getResponseBody(
			response.getStatusCode(),
			_srv->getErrorPage(response.getStatusCode())));
	}

	// delete _url;
	return (response);
}
