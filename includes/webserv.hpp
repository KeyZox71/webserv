/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:29:21 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/14 14:25:35 by adjoly           ###   ########.fr       */
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

#define BUFFER_SIZE 4096

namespace webserv {

} //-namespace webserv

#endif // __WEBSERV_WEBSERV_HPP__
