/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:17:48 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/29 15:48:19 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
#include <webserv.hpp>

class URL {
  public:
	URL(const std::string &url) : _full_url(url) { parse(); }

	bool operator==(const URL &other) const {
		return comparePathSegments(other);
	}

	bool operator<(const URL &other) const {
		return _full_url < other._full_url;
	}

	std::vector<std::string> getSegments(void) { return _path_segments; }
	std::string				 getFullUrl(void) const { return _full_url; }
	std::string getQueryString(void) const { return _query_string; }

  private:
	void parse() {
		size_t scheme_pos = _full_url.find("://");
		size_t path_start = 0;
		size_t query_start = _full_url.find('?');
		size_t port_start = _full_url.find(':', scheme_pos + 3);
		size_t port_end = _full_url.find('/', port_start);

		if (scheme_pos != std::string::npos) {
			path_start = _full_url.find('/', scheme_pos + 3);
		} else {
			path_start = 0;
		}

		if (query_start != std::string::npos) {
			_query_string = _full_url.substr(query_start + 1);
			if (path_start != std::string::npos) {
				std::string path =
					_full_url.substr(path_start, query_start - path_start);
				splitPath(path, _path_segments);
			}
		} else {
			if (path_start != std::string::npos) {
				std::string path = _full_url.substr(path_start);
				splitPath(path, _path_segments);
			}
		}

		if (port_start != std::string::npos && port_end != std::string::npos) {
			_port = _full_url.substr(port_start + 1, port_end - port_start - 1);
		}
	}

	void splitPath(const std::string		&path,
				   std::vector<std::string> &segments) const {
		std::stringstream ss(path);
		std::string		  segment;
		while (std::getline(ss, segment, '/')) {
			if (!segment.empty()) {
				segments.push_back(segment);
			}
		}
	}

	bool comparePathSegments(const URL &other) const {
		size_t min_size =
			std::min(_path_segments.size(), other._path_segments.size());
		for (size_t i = 0; i < min_size; ++i) {
			if (_path_segments[i] != other._path_segments[i]) {
				return false;
			}
		}
		return true;
	}

	std::string				 _full_url;
	std::vector<std::string> _path_segments;
	std::string				 _query_string;
	std::string				 _port;
};

inline std::ostream &operator<<(std::ostream &os, const URL &URL) {

	os << URL.getFullUrl();
	return os;
}
