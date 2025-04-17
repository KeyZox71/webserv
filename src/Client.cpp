/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 14:44:55 by adjoly            #+#    #+#             */
/*   Updated: 2025/04/15 18:38:42 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/default.hpp>
#include <netinet/in.h>
#include <server/Client.hpp>

Client::Client(int fd, sockaddr_in sockData, config::Config *conf)
	: _fd(fd), _client_addr(sockData) {
	
}
