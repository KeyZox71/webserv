/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:17:34 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/23 17:27:45 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "requests/HttpIMessage.hpp"
#include <map>
#include <string>

namespace webserv {


class Cgi {
	public:
		Cgi(http::IMessage &);
		~Cgi(void);

		void	initEnvp(void);
		
	protected:
	private:
		std::map<std::string, std::string> _envp;


		std::string	_request;
};
};
