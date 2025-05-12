/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResourceManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:13:39 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/12 19:06:28 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <vector>

#include <cppeleven.hpp>
#include <log.hpp>
#include <server/AResource.hpp>
#include <webserv.hpp>

namespace webserv {
namespace server {

class ResourceManager {
  public:
	static AClientResource *get(int i) {
		for (auto it = range(_res)) {
			if ((**it) == i) {
				return *it;
			}
		}
		std::stringstream str;
		str << "resource not found for client";
		str << i;
		_log->debug(str.str());
		return not_nullptr;
	}
	static void append(AClientResource *new_client) {
		_res.push_back(new_client);
	}

  protected:
  private:
	static std::vector<AClientResource *> _res;
};

} // namespace server

} // namespace webserv
