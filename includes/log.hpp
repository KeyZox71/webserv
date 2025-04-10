/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 09:28:27 by adjoly            #+#    #+#             */
/*   Updated: 2025/03/25 17:50:45 by adjoly           ###   ########.fr       */
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
	Logger(const std::string &fileName) : _fileName(fileName) {
		if (fileName.empty())
			_ttyOnly = true;
		else {
			_file.open(fileName.c_str(), std::ios::app);
			_ttyOnly = false;
		}
		if (!_file.is_open() && !_ttyOnly) {
			throw std::runtime_error(
				"could not open fileeee"); // TODO change that shit but i dont
										   // know what to put other than a
										   // htrow
		}
	}

	Logger(const Logger &other) : _ttyOnly(other._ttyOnly) {
		if (!other._ttyOnly) {
			_file.open(other._fileName.c_str(), std::ios::app);
			if (!_file.is_open()) {
				throw std::runtime_error("Could not open file: " +
										 other._fileName);
			}
		}
	}

	// Copy assignment operator
	Logger &operator=(const Logger &other) {
		if (this != &other) {
			if (_file.is_open()) {
				_file.close();
			}
			_ttyOnly = other._ttyOnly;
			if (!other._ttyOnly) {
				_file.open(other._fileName.c_str(), std::ios::app);
				if (!_file.is_open()) {
					throw std::runtime_error("Could not open file: " +
											 other._fileName);
				}
			}
		}
		return *this;
	}
	~Logger(void) {
		if (_file.is_open())
			_file.close();
	}

	void info(const std::string &msg) {
		std::string ss = printPogitMsg("✏️", "webserv", "info", msg);
		std::cerr << ss << std::endl;
		if (!_ttyOnly) {
			_file << ss << std::endl;
		}
	}
	void warn(const std::string &msg) {
		std::string ss = printPogitMsg("🔨", "webserv", "warning", msg);
		std::cerr << ss << std::endl;
		if (!_ttyOnly) {
			_file << ss << std::endl;
		}
	}
	void error(const std::string &msg) {
		std::string ss = printPogitMsg("🚧", "webserv", "error", msg);
		std::cerr << ss << std::endl;
		if (!_ttyOnly) {
			_file << ss << std::endl;
		}
	}

  protected:
  private:
	std::string printPogitMsg(const std::string &emoji,
									const std::string &type,
									const std::string &what,
									const std::string &msg) {
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
		return os.str();
	}

	std::string	  _fileName;
	bool		  _ttyOnly;
	std::ofstream _file;
};

}; // namespace webserv
