/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PfdManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:01:01 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 18:12:13 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <webserv.hpp>

#include <poll.h>
#include <vector>

namespace webserv {
namespace server {

enum pfd_type { SRV, CLIENT, RES };

class ComparePfd {
  public:
	ComparePfd(int id) : _id(id) {}

	bool operator()(const struct pollfd res) const { return res.fd == _id; }

  private:
	int _id;
};

class PfdManager {
  public:
	static struct pollfd at(int i) { return _pfd_vec[i]; }

	static struct pollfd *get(int id) {
		auto it =
			std::find_if(_pfd_vec.begin(), _pfd_vec.end(), ComparePfd(id));

		if (it != _pfd_vec.end())
			return &*it;
		throw std::out_of_range("pollfd does not exist for client D:");
	}

	static pfd_type getType(int id) {
		auto it =
			std::find_if(_pfd_vec.begin(), _pfd_vec.end(), ComparePfd(id));

		if (it != _pfd_vec.end()) {
			size_t i = std::distance(_pfd_vec.begin(), it);
			try {
				auto it = _pfd_type.at(i);
				return it;
			} catch (std::out_of_range &) {
				throw std::out_of_range("pollfd does not exist for client D:");
			}
		}
		throw std::out_of_range("pollfd does not exist for client D:");
	}

	static void append(struct pollfd pfd, pfd_type type) {
		_pfd_vec.push_back(pfd);
		_pfd_type.push_back(type);
	}

	static void remove(int id) {
		auto it =
			std::find_if(_pfd_vec.begin(), _pfd_vec.end(), ComparePfd(id));

		if (it != _pfd_vec.end()) {
			auto type = _pfd_type.begin() + std::distance(_pfd_vec.begin(), it);
			_pfd_type.erase(type);
			close(it->fd);
			_pfd_vec.erase(it);
		}
	}

	static struct pollfd *			   data(void) { return _pfd_vec.data(); }
	static size_t					   size(void) { return _pfd_vec.size(); }
	static std::vector<struct pollfd> *vec(void) { return &_pfd_vec; }

	static void clear(void) {
		for (auto it = range(_pfd_vec)) {
			close(it->fd);
		}
		_pfd_vec.clear();
		_pfd_type.clear();
	}

  protected:
  private:
	static std::vector<struct pollfd> _pfd_vec;
	static std::vector<pfd_type>	  _pfd_type;
};

} // namespace server
} // namespace webserv
