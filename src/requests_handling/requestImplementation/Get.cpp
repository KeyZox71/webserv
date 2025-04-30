/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:40:16 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/30 15:30:08 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

#include <requests/default.hpp>

using namespace webserv::http;

Get::Get(std::string &data) { this->parse(data); }

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

	std::ostringstream body_stream;
	while (std::getline(stream, line))
		body_stream << line << "\n";
	this->_body = body_stream.str();

	_url = new URL("http://" + _headers["Host"] + _target);

	/*std::cout << "wtf = " << _headers["Host"] << std::endl;
	std::cout << *_url << std::endl;*/

	/*
	std::cout << "-- start-line --" << std::endl;
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "target: " << this->_target << std::You can use every macro and
define like FD_SET, FD_CLR, FD_ISSET and, FD_ZERO (understanding what they do
and how they work is very useful). • A request to your server should never hang
indefinitely. • Your server must be compatible with standard web browsers of
your choice. • We will consider that NGINX is HTTP 1.1 compliant and may be used
to compare headers and answer behaviors. • Your HTTP response status codes must
be accurate. • Your server must have default error pages if none are provided.
• You can’t use fork for anything other than CGI (like PHP, or Python, and so
forth). • You must be able to serve a fully static website. • Clients must be
able to upload files. • You need at least the GET, POST, and DELETE methodendl;
	std::cout << "protocol: " << this->_protocol << std::endl;
	std::cout << std::endl;
	std::cout << "-- headers --" << std::endl;
	for (std::map<std::string, std::string>::const_iterator it =
this->_headers.begin(); it != this->_headers.end(); ++it) std::cout << it->first
<< ": " << it->second << std::endl; std::cout << std::endl; std::cout << "--
body --" << std::endl << this->_body << std::endl;
	*/
}

char isDirectory(const std::string &path) {
	struct stat file_stat;

	if (stat(path.c_str(), &file_stat) != 0) {
		throw std::runtime_error("can't open file (non-existant ?)");
	}
	return S_ISDIR(file_stat.st_mode);
}

Response Get::execute(void) {
	http::Response response;

	try {
		if (isDirectory(this->_target)) {
			DIR						*dir;
			struct dirent			*entry;
			struct stat				 file_stat;
			std::vector<std::string> files;

			if ((dir = opendir(this->_target.c_str())) == NULL)
				throw;
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

#ifdef VERBOSE
			//_log->debug(response.str().c_str());
#endif
		}
	} catch (...) {
		// TODO: replace with a predefined array of error pages
		response.setProtocol(this->_protocol);
		response.setStatusCode(404);
		response.addHeader("Content-Type", "text/html");
		response.setBody(
			http::Errors::getResponseBody(response.getStatusCode()));
	}

	return (response);
}
