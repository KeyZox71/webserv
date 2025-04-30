/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:51:46 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/30 14:30:00 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

#include <requests/Response.hpp>

namespace webserv {
namespace http {

class Errors {
public:
	static std::string	getResponseBody(int error_code);
	static void			setEntries(const std::map<int, std::string>);

	static std::map<int, std::string>	message;

private:
	static std::map<int, std::string>	populateMessages();

	static std::map<int, std::string>	set_error_pages;

};

} // -namespace http
} // -namespace webserv
