/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:29:21 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/22 11:54:13 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/types.h>
#ifndef __WEBSERV_WEBSERV_HPP__
# define __WEBSERV_WEBSERV_HPP__


#include <string>
#include <cstring>
#include <fstream>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define auto __auto_type
#define range(x) x.begin(); it != x.end(); it++
#define prange(x) x->begin(); it != x->end(); it++

#define BUFFER_SIZE 4096

namespace webserv {

} //-namespace webserv

#endif // __WEBSERV_WEBSERV_HPP__
