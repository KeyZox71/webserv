/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   URL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:17:48 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/22 12:42:21 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstring>
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

	bool operator<(const URL &other) const { return _full_url < other._full_url; }

	std::vector<std::string> getSegments(void) { return _path_segments; }
	std::string				 getFullUrl(void) { return _full_url; }

  private:
	void parse() {
		size_t scheme_pos = _full_url.find("://");
		size_t path_start = 0;

		if (scheme_pos != std::string::npos) {
			path_start = _full_url.find('/', scheme_pos + 3);
		} else {
			path_start = 0;
		}

		if (path_start != std::string::npos) {
			std::string path = _full_url.substr(path_start);
			splitPath(path, _path_segments);
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
};
