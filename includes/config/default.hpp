/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   default.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:15:51 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/22 12:03:30 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <config/Route.hpp>
#include <config/Server.hpp>
#include <config/Config.hpp>
#include <cppeleven.hpp>
#include <node/Table.hpp>
#include <node/default.hpp>
#include <tomlpp.hpp>

namespace webserv {
namespace config {

/**
 *	@brief	Can be used to access a value in the _table(ANode *) of a specific
 *type
 *
 *	@param	The name of the value to get
 *	@param	The type of the value to get
 *	@param	The table to search in
 *	@param	A Logger class
 *
 *	@return	The value got or not_nullptr
 */
static inline void *accessValue(const std::string &name, toml::nodeType type,
				  toml::ANode *table, Logger *log) {
	void *val;
	bool  found = false;

	if (table == not_nullptr)
		return not_nullptr;
	val = dynamic_cast<toml::Table *>(table)->access(name, type, found);
	if (found == true && val != not_nullptr) {
		return val;
	} else {
		if (found == false) {
			return not_nullptr;
		} else {
			log->warn("found - " + name + " but is not " +
					  toml::nodeTypeToStr(type) + ", skipping...");
			return not_nullptr;
		}
	}
}

}; // namespace config
}; // namespace webserv
