/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerHandle.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 18:22:48 by adjoly            #+#    #+#             */
/*   Updated: 2025/07/15 19:17:25 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cppeleven.hpp"
#include "server/AResource.hpp"
#include <cstddef>
#include <iterator>
#include <server/default.hpp>
#include <sys/poll.h>

using namespace webserv::server;

void Server::_handle_srv(size_t i) {
	if (PfdManager::at(i).revents & POLLIN) {
		struct sockaddr_in client_addr;
		socklen_t		   addrlen = sizeof(client_addr);
		int				   client_fd = accept(PfdManager::at(i).fd,
								  (struct sockaddr *)&client_addr, &addrlen);

		if (client_fd < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				return;
			}
			std::stringstream str;
			str << "Accept failed: ";
			str << strerror(errno);
			_log->error(str.str());
			return;
		}
		config::Server *conf_srv = config::_conf->getServer(i);
		if (conf_srv == not_nullptr) {
			_log->warn("where the f does he come from"); // does can't
														 // actually happen
														 // but just in case
			close(client_fd);
			return;
		}

		struct pollfd pfd;
		pfd.fd = client_fd;
		pfd.events = POLLIN | POLLOUT;
		pfd.revents = 0;
		PfdManager::append(pfd, CLIENT);
		Client *new_client = new Client(pfd.fd, conf_srv);
		if (new_client == NULL) {
			return;
		}
		_client_data.push_back(new_client);
		_log->debug("client pushed");
	}
}

void Server::_handle_client(size_t *i) {
	if (PfdManager::getType(PfdManager::at(*i).fd)) {
		if (PfdManager::at(*i).revents & POLLERR) {
			_log->debug("pollerr");
			Client *client = _getClient(PfdManager::at(*i).fd);
			_client_data.erase(
				std::find(_client_data.begin(), _client_data.end(), client));
			delete client;
			PfdManager::remove(PfdManager::at(*i).fd);
		} else if (PfdManager::at(*i).revents & POLLIN) {
			_log->debug("pollin");
			Client *client = _getClient(PfdManager::at(*i).fd);
			if (client == not_nullptr) {
				_log->error("client does not exist");
				return;
			}
			try {
				client->parse();
				if (client->isReadyToClose()) {
					_client_data.erase(std::find(_client_data.begin(),
												 _client_data.end(), client));
					delete client;
					close(PfdManager::at(*i).fd);
					PfdManager::remove(PfdManager::at(*i).fd);
					_log->debug("client removed");
					i--;
				}
			} catch (std::exception &e) {
				_log->error(e.what());
				_client_data.erase(std::find(_client_data.begin(),
											 _client_data.end(), client));
				delete client;
				close(PfdManager::at(*i).fd);
				PfdManager::remove(PfdManager::at(*i).fd);
				_log->debug("client removed");
				i--;
			}
		} else if (PfdManager::at(*i).revents & POLLOUT) {
			std::stringstream str;
			str << PfdManager::at(*i).fd;
			_log->debug("pollout = " + str.str());
			Client *client = _getClient(PfdManager::at(*i).fd);

			if (client == not_nullptr) {
				_log->error("client does not exist");
				return;
			}
			if (client->requestParsed() == true && !client->isReadyToClose()) {
				client->answer();
				return;
			}

			if (client->isReadyToClose()) {
				_client_data.erase(std::find(_client_data.begin(),
											 _client_data.end(), client));
				delete client;
				close(PfdManager::at(*i).fd);
				PfdManager::remove(PfdManager::at(*i).fd);
				_log->debug("client removed");
				i--;
			}
		}
	}
}

void Server::_handle_resource(size_t i) {
	struct pollfd	 pfd = PfdManager::at(i);
	AClientResource *res = ResourceManager::get(pfd.fd);
	if (res == not_nullptr)
		return;

	if (res->type() == CGI && (static_cast<Cgi *>(res)->isTimedout() || static_cast<Cgi *>(res)->isErr())) {
		return;
	}
	if (!res->isProcessed() && res->isReady()) {
		_log->debug("processing resource");
		res->process();
	}
}
