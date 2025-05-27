/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:17:48 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 19:46:54 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "log.hpp"
#include <cstring>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <webserv.hpp>

class URL {
  public:
	URL(const std::string &url) : _full_url(url) { _parse(); }

	bool operator<(const URL &other) const {
		return _full_url < other._full_url;
	}

	int countMatchingSegments(const URL &url) const {
		if (_path_segments.size() == 0 || url._path_segments.size() == 0)
			return 0;
		int i = 0;

		auto u = url._path_segments;
		for (auto it = u.begin(); it != u.end() && *it == _path_segments[i];
			 it++, i++)
			;

		return i;
	}

	std::vector<std::string> getSegments(void) const { return _path_segments; }

	std::string getFullUrl(void) const { return _full_url; }
	std::string getQueryString(void) const { return _query_string; }
	std::string getPort(void) const { return _port; }

  private:
	void _parse() {
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
				_splitPath(path, _path_segments);
			}
		} else {
			if (path_start != std::string::npos) {
				std::string path = _full_url.substr(path_start);
				_splitPath(path, _path_segments);
			}
		}

		if (port_start != std::string::npos && port_end != std::string::npos) {
			_port = _full_url.substr(port_start + 1, port_end - port_start - 1);
		}
	}

	void _splitPath(const std::string &		  path,
					std::vector<std::string> &segments) const {
		std::stringstream ss(path);
		std::string		  segment;
		while (std::getline(ss, segment, '/')) {
			if (!segment.empty()) {
				segments.push_back(segment);
			}
		}
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
