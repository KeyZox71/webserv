/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:27 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/20 14:55:09 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace webserv {
class Logger {
  public:
	Logger(std::string fileName) {
		if (fileName.empty())
			_ttyOnly = true;
		else {
			_file.open(fileName.c_str(), std::ios::app);
			_ttyOnly = false;
		}
		if (!_file.is_open()) {
			throw std::runtime_error("could not open fileeee"); // TODO change that shit but i dont know what to put other than a htrow
		}
	}

	~Logger(void) { _file.close(); }

	void info(std::string msg) {
		std::stringstream ss = printPogitMsg("✏️", "webserv", "info", msg);
		std::cerr << ss << std::endl;
		if (!_ttyOnly) {
			_file << ss << std::endl;
		}
	}
	void warn(std::string msg) {
		std::stringstream ss = printPogitMsg("🔨", "webserv", "warning", msg);
		std::cerr << ss << std::endl;
		if (!_ttyOnly) {
			_file << ss << std::endl;
		}

	}
	void error(std::string msg) {
		std::stringstream ss = printPogitMsg("🚧", "webserv", "error", msg);
		std::cerr << ss << std::endl;
		if (!_ttyOnly) {
			_file << ss << std::endl;
		}
	}

  protected:
  private:
	std::stringstream printPogitMsg(std::string emoji, std::string type, std::string what, std::string msg) {
		std::stringstream os;
#ifdef tty
		if (what.empty())
			os << type << ":" << msg;
		else
			os << type << "(" << what << "):" << msg;
#else
		if (what.empty())
			os << "「" << emoji << "」" << type << ":" << msg;
		else
			os << "「" << emoji << "」" << type << "(" << what << "):" << msg;
#endif
		return os;
	}
	bool		  _ttyOnly;
	std::ofstream _file;
};

}; // namespace webserv
