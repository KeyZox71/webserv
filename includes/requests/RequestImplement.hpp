/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestImplement.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:30:15 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/27 16:49:00 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/ARequest.hpp>

namespace webserv {
namespace server {
class Cgi;
class CgiIn;
}; // namespace server
namespace http {

class Get : public ARequest {
  public:
	Get(void) {}
	Get(std::string &data);

	void parse(std::string const &data);

	Response execute(void);

	//   private:
	// server::Cgi *_cgi;
};

class Post : public ARequest {
  public:
	Post(void) {}
	Post(std::string &data);

	void parse(std::string const &data);

	std::string extractFilename(const std::string &header);
	void		handleMultipartData(const std::string &body,
									const std::string &boundary);

	Response execute(void);

	//   private:
	// server::Cgi *_cgi;
};

class Delete : public ARequest {
  public:
	Delete(void) {}
	Delete(std::string &data);

	void parse(std::string const &data);

	Response execute(void);
};

}; // namespace http
}; // namespace webserv
