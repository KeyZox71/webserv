/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResource.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:20:09 by mmoussou          #+#    #+#             */
/*   Updated: 2025/05/12 19:05:51 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <poll.h>

namespace webserv {
namespace server {

class AClientResource {
public:
	virtual			~AClientResource() {}

	bool	operator==(int i) const {
		if (i == _client_id)
			return true;
		return false;
	}

	void			addFileDescriptor(struct pollfd fd);
	struct pollfd	getFileDescriptor();

private:
	struct pollfd	_fd;

	int	_client_id;

};

} // -namepsace server
} // -namespace webserv
