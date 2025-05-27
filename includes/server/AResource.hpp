/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:20:09 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/27 13:12:03 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <poll.h>

namespace webserv {
namespace server {

enum clientResType { CGI_IN, CGI, UP_FILE };

class AClientResource {
  public:
	virtual ~AClientResource() {}

	bool operator==(int i) const {
		if (i == _fd)
			return true;
		return false;
	}

	virtual clientResType type(void) const = 0;
	int					  getId(void) const { return _fd; }

	virtual void  process(void) = 0;
	bool		  isProcessed(void) const { return _processed; }
	virtual short event(void) const = 0;

  protected:
	int	  _fd;
	bool  _processed;
	short _pfd_event;
};

} // namespace server
} // namespace webserv
