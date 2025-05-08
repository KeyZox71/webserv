/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adjoly <adjoly@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:43:54 by adjoly            #+#    #+#             */
/*   Updated: 2025/05/08 11:54:17 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifdef PKGS
#define SAMPLE_CONF_PATH "./sample.toml"
#else
#define SAMPLE_CONF_PATH "/etc/webserv/default.conf"
#endif
#define WEBSRV_VERSION "v0.2"

bool help(int, char **);
