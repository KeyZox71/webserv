/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:13:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/07/16 10:39:39 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <sys/poll.h>
#include <vector>

#include <cppeleven.hpp>
#include <log.hpp>
#include <server/AResource.hpp>
#include <webserv.hpp>

namespace webserv {
namespace server {

class CompareId {
  public:
	CompareId(int id) : _id(id) {}

	bool operator()(const webserv::server::AClientResource *resource) const {
		return resource->getId() == _id;
	}

  private:
	int _id;
};


class ResourceManager {
  public:
	/**
	 *	@brief	Can be used to get a resource correcponding to the id of the
	 *			resource
	 *
	 *	@param	The id of the resource
	 *
	 *	@return	The resource
	 *	@throw	std::out_of_range If the resource does not exist
	 */
	static AClientResource *get(int id) {
		auto it = std::find_if(_res.begin(), _res.end(), CompareId(id));

		if (it != _res.end())
			return (*it);
		throw std::out_of_range("resource does not exist for client D:");
	}

	/**
	 *	@brief	Can be used to append a resource to the ResourceManager
	 *
	 *	@param	The resource to add
	 */
	static void append(AClientResource *new_client) {
		_res.push_back(new_client);
	}

	/**
	 *	@brief	Can be used to remove a resource by giving it it's id
	 *
	 *	@param	The id of the resource to remove
	 */
	static void remove(int id) {
		auto it = std::find_if(_res.begin(), _res.end(), CompareId(id));

		if (it != _res.end()) {
			auto oh = *it;
			_res.erase(it);
			delete oh;
		}
	}

	static void clear(void) {
		for (auto it = range (_res)) {
			delete *it;
		}
	}

  protected:
  private:
	static std::vector<AClientResource *> _res;
};

} // namespace server

} // namespace webserv
