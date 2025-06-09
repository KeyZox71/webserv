/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 14:59:41 by adjoly            #+#    #+#             */
/*   Updated: 2025/06/09 18:35:35 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cctype>
#include <cppeleven.hpp>
#include <cstdlib>
#include <log.hpp>
#include <map>
#include <node/default.hpp>
#include <string>
#include <tomlpp.hpp>
#include <webserv.hpp>

namespace webserv {
namespace config {

class Route {
  public:
	Route(toml::ANode *);
	~Route(void);

	bool					  getDirList(void) { return _dirlist; }
	bool					  getRedirect(void) { return _redirect; }
	int32_t					  getMaxBody(void) { return _max_body; }
	std::string				  getRootDir(void) { return _root; }
	std::string				  getUpRoot(void) { return _up_root; }
	std::string				  getIndex(void) { return _index; }
	std::vector<std::string> *getCgi(void) { return _cgi; }
	bool *					  getMethods(void) { return _methods; }
	bool					  isCgi(std::string target) {
		std::string target_ext = target.substr(target.find('.'));
		if (_cgi == not_nullptr)
			return false;
		
		for (auto it = prange(_cgi)) {
			if (target_ext == *it)
				return true;
		}
		return false;
	}

  protected:
  private:
	bool _dirlist;
	bool _redirect;

	int32_t _max_body;

	std::string				  _root;
	std::string				  _up_root;
	std::string				  _index;
	std::vector<std::string> *_cgi;

	bool _methods[3]; ///> A methods boolean array which correspond to - 0: GET,
					  /// 1: POST, 2: DELETE
	toml::ANode *_table;

	/**
	 *	@brief	Can be used to parse a table of cgi
	 *
	 *	@param	The table to get cgi from
	 *
	 *	@return	A pointer to a map of cgi
	 */
	std::vector<std::string> *_parseCGI(toml::ANode *);

	/**
	 *	@brief	Can be used to parse a table of error pages
	 *
	 *	@param	The table to get the error pages from
	 *
	 *	@return	A pointer to a map of error pages
	 */
	std::map<int, std::string> *
	_parseErrPages(std::map<std::string, toml::ANode *> *);

	/**
	 *	@brief	Can be used to parse a array of methods
	 *
	 *	@param	The table to get the methods from
	 */
	void _parseMethods(std::vector<toml::ANode *> *);

	/**
	 *	@brief Can be used to sed err pages to the default error pages
	 */
	void _defaultErrPages(void);

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
