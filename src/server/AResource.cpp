/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AResource.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:42:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/29 15:45:19 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/AResource.hpp>

using namespace webserv;
using namespace server;

void	AClientResource::addFileDescriptor(struct pollfd fd)
{
	this->_fd = fd;
}


struct pollfd	AClientResource::getFileDescriptor()
{
	return (this->_fd);
}
