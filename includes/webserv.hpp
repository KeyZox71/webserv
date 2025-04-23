/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:29:21 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/22 14:27:31 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define auto __auto_type

#include <sys/types.h>
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

