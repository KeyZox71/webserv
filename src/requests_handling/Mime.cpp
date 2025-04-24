/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 14:50:37 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/24 15:24:08 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <requests/Mime.hpp>

using namespace webserv;

std::map<std::string, std::string> http::Mime::initMimeTypes() {
	std::map<std::string, std::string> types;

	types["aac"] = "audio/aac";
	types["abw"] = "application/x-abiword";
	types["apng"] = "image/apng";
	types["arc"] = "application/x-freearc";
	types["avif"] = "image/avif";
	types["avi"] = "video/x-msvideo";
	types["azw"] = "application/vnd.amazon.ebook";
	types["bin"] = "application/octet-stream";
	types["bmp"] = "image/bmp";
	types["bz"] = "application/x-bzip";
	types["bz2"] = "application/x-bzip2";
	types["cda"] = "application/x-cdf";
	types["csh"] = "application/x-csh";
	types["css"] = "text/css";
	types["csv"] = "text/csv";
	types["doc"] = "application/msword";
	types["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	types["eot"] = "application/vnd.ms-fontobject";
	types["epub"] = "application/epub+zip";
	types["gz"] = "application/gzip";
	types["gif"] = "image/gif";
	types["htm"] = "text/html";
	types["html"] = "text/html";
	types["ico"] = "image/vnd.microsoft.icon";
	types["ics"] = "text/calendar";
	types["jar"] = "application/java-archive";
	types["jpeg,"] = "image/jpeg";
	types["js"] = "text/javascript";
	types["json"] = "application/json";
	types["jsonld"] = "application/ld+json";
	types["md"] = "text/plain";
	types["mid"] = "audio/midi";
	types["midi"] = "audio/midi";
	types["mjs"] = "text/javascript";
	types["mp3"] = "audio/mpeg";
	types["mp4"] = "video/mp4";
	types["mpeg"] = "video/mpeg";
	types["mpkg"] = "application/vnd.apple.installer+xml";
	types["odp"] = "application/vnd.oasis.opendocument.presentation";
	types["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	types["odt"] = "application/vnd.oasis.opendocument.text";
	types["oga"] = "audio/ogg";
	types["ogv"] = "video/ogg";
	types["ogx"] = "application/ogg";
	types["opus"] = "audio/ogg";
	types["otf"] = "font/otf";
	types["png"] = "image/png";
	types["pdf"] = "application/pdf";
	types["php"] = "application/x-httpd-php";
	types["ppt"] = "application/vnd.ms-powerpoint";
	types["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	types["rar"] = "application/vnd.rar";
	types["rtf"] = "application/rtf";
	types["sh"] = "application/x-sh";
	types["svg"] = "image/svg+xml";
	types["tar"] = "application/x-tar";
	types["tif,"] = "image/tiff";
	types["ts"] = "video/mp2t";
	types["ttf"] = "font/ttf";
	types["txt"] = "text/plain";
	types["vsd"] = "application/vnd.visio";
	types["wav"] = "audio/wav";
	types["weba"] = "audio/webm";
	types["webm"] = "video/webm";
	types["webp"] = "image/webp";
	types["woff"] = "font/woff";
	types["woff2"] = "font/woff2";
	types["xhtml"] = "application/xhtml+xml";
	types["xls"] = "application/vnd.ms-excel";
	types["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	types["xml"] = "application/xml";
	types["xul"] = "application/vnd.mozilla.xul+xml";
	types["zip"] = "application/zip";
	types["3gp"] = "video/3gpp";
	types["3g2"] = "video/3gpp2";
	types["7z"] = "application/x-7z-compressed";
	types["nix"] = "text/plain";

	return types;
}

std::map<std::string, std::string> http::Mime::mimeTypes = Mime::initMimeTypes();

std::string http::Mime::getType(const std::string &filename) {
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos)
        return "text/plain"; //default

    std::string ext = filename.substr(dot_pos + 1);
    std::map<std::string, std::string>::const_iterator it = mimeTypes.find(ext);
    if (it != mimeTypes.end())
        return it->second;
    return "application/octet-stream"; //unknown extension so default
}
