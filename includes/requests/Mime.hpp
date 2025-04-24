/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:53:25 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/24 15:10:32 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <map>

namespace webserv {
namespace http {

class Mime {
public:
    static std::string getType(const std::string &filename);

private:
    static std::map<std::string, std::string> initMimeTypes();

    static std::map<std::string, std::string> mimeTypes;
};

} // -namespace http
} // -namespace webserv
