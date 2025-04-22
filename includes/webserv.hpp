/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:29:21 by mmoussou          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/22 11:54:13 by mmoussou         ###   ########.fr       */
=======
/*   Updated: 2025/04/22 12:32:23 by adjoly           ###   ########.fr       */
>>>>>>> 8129f45 (「🏗️」 wip: added url class for easier manipulation)
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/types.h>
#ifndef __WEBSERV_WEBSERV_HPP__
#define __WEBSERV_WEBSERV_HPP__

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <sstream>

#define auto __auto_type
#define range(x)                                                               \
	x.begin();                                                                 \
	it != x.end();                                                             \
	it++
#define prange(x)                                                              \
	x->begin();                                                                \
	it != x->end();                                                            \
	it++

#define BUFFER_SIZE 4096

namespace webserv {

} // namespace webserv

#endif // __WEBSERV_WEBSERV_HPP__
