/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:59:41 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/24 10:48:37 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "cppeleven.hpp"
#include "log.hpp"
#include "node/default.hpp"
#include <cctype>
#include <cstdlib>
#include <map>
#include <string>
#include <tomlpp.hpp>

namespace webserv {
namespace config {

class Route {
  public:
	Route(toml::ANode *, Logger *);
	~Route(void);

  protected:
  private:
	bool _dirlist;
	bool _cookies;
	bool _uploads;
	bool _redirect;

	int32_t _max_body;

	std::string							_root;
	std::string							_upRoot;
	std::map<std::string, std::string> *_cgi;

	Logger *_log;

	bool						_methods[3]; ///> A methods boolean array which correspond to - 0: GET, 1: POST, 2: DELETE
	std::map<int, std::string> *_err_pages; ///> An error pages map to map error specified in the config file
	toml::ANode				   *_table;

	/**
	 *	@brief	Can be used to access a value in the _table(ANode *) of a specific type
	 *
	 *	@param	The name of the value to get
	 *	@param	The type of the value to get
	 *
	 *	@return	The value got or not_nullptr
	 */
	void   *accessValue(std::string, toml::nodeType);


	/**
	 *	@brief	Can be used to parse a table of cgi
	 *
	 *	@param	The table to get cgi from
	 *
	 *	@return	A pointer to a map of cgi
	 */
	std::map<std::string, std::string> *_parseCGI(std::map<std::string, toml::ANode *> *);

	/**
	 *	@brief	Can be used to parse a table of error pages
	 *
	 *	@param	The table to get the error pages from
	 *
	 *	@return	A pointer to a map of error pages
	 */
	std::map<int, std::string> *_parseErrPages(std::map<std::string, toml::ANode *> *);

	/**
	 *	@brief	Can be used to parse a array of methods
	 *
	 *	@param	The table to get the methods from
	 */
	void	_parseMethods(std::vector<toml::ANode *> *);

	/**
	 *	@brief Can be used to sed err pages to the default error pages
	 */
	void	_defaultErrPages(void);

	/**
	 *	@brief	Can be used to parse a string of a number with a size (ex. 10M)
	 *
	 *	@param	The input string
	 *
	 *	@return	The number in bytes
	 */
	int32_t _parseSize(std::string size) {
		if (size[size.size()] == 'M')
			return std::atoi(size.c_str()) * 1024 * 1024;
		if (size[size.size()] == 'K')
			return std::atoi(size.c_str()) * 1024;
		if (isalpha(size[size.size()]))
			return std::atoi(size.c_str());
		return -1;
	}
};

} // namespace config
} // namespace webserv
