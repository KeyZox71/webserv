/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectResp.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:12:04 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/04 13:33:36 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <requests/Response.hpp>

namespace webserv {
namespace http {
class Redirect : public Response {
  public:
	Redirect(std::string redir_path) : _path(redir_path) {
		setProtocol("HTTP/1.1");
		setStatusCode(308);
		addHeader("Location", _path);
		addHeader("Content-lengh", "0");
	}

	void		setPath(const std::string path) { _path = path; }
	std::string getPath(void) const { return _path; }

	~Redirect(void) {}

  protected:
  private:
	std::string _path;
};
} // namespace http
} // namespace webserv
