/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:20:09 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/23 18:27:06 by adjoly           ###   ########.fr       */
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
		if (i == _res_id)
			return true;
		return false;
	}

	void		  setFileDescriptor(struct pollfd *fd) { _fd = fd; }
	struct pollfd getFileDescriptor(void) const { return *_fd; }

	virtual clientResType type(void) const = 0;
	int					  getId(void) const { return _res_id; }

  protected:
	struct pollfd *_fd;

	int _res_id;
};

} // namespace server
} // namespace webserv
