/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/03/25 17:10:29 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Server.hpp"
#include <tomlpp.hpp>

int main(int, char **) {
	webserv::config::Server serverConf("exemples/test.toml");
	return 0;
}
