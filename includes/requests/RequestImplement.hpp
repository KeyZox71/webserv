/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestImplement.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 09:30:15 by adjoly            #+#    #+#             */
/*   Updated: 2025/07/10 16:51:32 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstddef>
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
	Get(std::string &data, config::Server *srv);
	~Get(void);

	void parse(std::string const &data);

	Response execute(void);

	server::Cgi *getCgi() const { return _cgi; }

  private:
	server::Cgi *_cgi;
};

class Post : public ARequest {
  public:
	Post(std::string &data, config::Server *srv);
	~Post(void);

	void parse(std::string const &data);

	Response execute(void);

	server::Cgi *getCgi() const { return _cgi; }

  private:
	std::string extractFilename(const std::string &header);
	void		handleMultipartData(const std::string &body,
									const std::string &boundary);
	void	handleBinaryUpload();
	server::Cgi *_cgi;
};

class Delete : public ARequest {
  public:
	Delete(void) {}
	Delete(std::string &data, config::Server *srv);
	~Delete();

	void parse(std::string const &data);

	Response execute(void);
};

}; // namespace http
}; // namespace webserv
