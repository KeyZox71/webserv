/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/24 11:06:33 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"

namespace webserv {
namespace server {

class FileUpload : public AClientResource {
  public:
	FileUpload(int id) { 
		_fd->fd = id;
		_fd->events = POLLOUT;
	}
	~FileUpload(void) {}

	clientResType type(void) const { return UP_FILE; }

  protected:
  private:
};

} // namespace server
} // namespace webserv
