/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiIn.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:14:45 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/23 12:51:22 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "server/AResource.hpp"

namespace webserv {
namespace server {

class CgiIn: public AClientResource {
  public:
	CgiIn(int id) { _res_id = id; }
	~CgiIn(void) {}

	clientResType type(void) const { return CGI_IN; }

  protected:
  private:
};

} // namespace server
} // namespace webserv
