/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:20:09 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/24 10:09:33 by adjoly           ###   ########.fr       */
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
		if (i == _fd->fd)
			return true;
		return false;
	}

	void		  setFileDescriptor(struct pollfd *fd) { _fd = fd; }
	struct pollfd getFileDescriptor(void) const { return *_fd; }

	virtual clientResType type(void) const = 0;
	int					  getId(void) const { return _fd->fd; }

  protected:
	struct pollfd *_fd;
};

} // namespace server
} // namespace webserv
