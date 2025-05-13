/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:20:09 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/13 09:56:02 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <poll.h>

namespace webserv {
namespace server {

enum	clientResType {
	CGI,
	UP_FILE
};

class AClientResource {
public:
	virtual			~AClientResource() {}

	bool	operator==(int i) const {
		if (i == _res_id)
			return true;
		return false;
	}

	void			addFileDescriptor(struct pollfd fd);
	struct pollfd	getFileDescriptor();

	virtual	clientResType type(void);

protected:
	struct pollfd	_fd;

	int	_res_id;

};

} // -namepsace server
} // -namespace webserv
