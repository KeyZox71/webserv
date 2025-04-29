/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Resource.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:42:01 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/29 14:56:53 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <server/Resource.hpp>

/*
class ClientResource {
public:
	virtual	void	handleResource() = 0;
	virtual			~HttpClientResource() {}

	void			addFileDescriptor(int fd);
	void			getFileDescriptors();

private:
	struct pollfd	fd;

}
*/

using namespace webserv;
using namespace server;

void	ClientResource::addFileDescriptor(struct pollfd fd)
{
	this->_fd = fd;
}


struct pollfd	ClientResource::getFileDescriptor()
{
	return (this->_fd);
}
