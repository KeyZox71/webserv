/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FileUpload.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/13 18:56:25 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"

namespace webserv {
namespace server {

class FileUpload : public AClientResource {
  public:
	FileUpload(int id) { _res_id = id; }
	~FileUpload(void) {}

	clientResType type(void) const { return UP_FILE; }

  protected:
  private:
};

} // namespace server
} // namespace webserv
