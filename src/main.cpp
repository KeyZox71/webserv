/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/04/10 12:18:39 by mmoussou         ###   ########.fr       */
/*   Updated: 2025/03/25 17:10:29 by adjoly           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <config/Server.hpp>
#include <tomlpp.hpp>
#include <webserv.hpp>
#include <requests/default.hpp>

#define PORT 8080
#define BUFFER_SIZE 4096

int server_socket;
int client_socket;

void	close_socket(int signal)
{
	std::cerr << std::endl << "closing..." << std::endl;
	close(client_socket);
	close(server_socket);
	exit(signal);
}

std::string getMethod(std::string &data)
{
	return (data.substr(0, data.substr(0, 4).find_last_not_of(" ") + 1));
}

int main()
{
	// handle ctrl-C to close server socket
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR || signal(SIGINT, close_socket) == SIG_ERR || signal(SIGQUIT, close_socket) == SIG_ERR)
	{
        std::cerr << "Error registering signal handlers!" << std::endl;
        return 1;
    }

    // create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
	{
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // prepare the server address
    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1)
	{
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }
    if (listen(server_socket, 5) == -1)
	{
        std::cerr << "Failed to listen on socket" << std::endl;
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true)
	{
        // accept an incoming connection
        sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        //int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
        client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // receive the HTTP request
        std::string received_data;
        char buffer[BUFFER_SIZE];
		ssize_t bytes_received;
		do
		{
        std::memset(buffer, 0, BUFFER_SIZE);
		bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == -1)
		{
            std::cerr << "Failed to receive request" << std::endl;
            close(client_socket);
            continue;
        }
        received_data += std::string(buffer, bytes_received);
		}
		while (buffer[bytes_received]);

        // parse the request

        // handle the request
		std::string	response;

		//std::cout << received_data << std::endl;
		std::cout << getMethod(received_data) << std::endl;

        if (getMethod(received_data) == "GET")
		{
			std::cout << "------------ GET REQUEST ------------" << std::endl;
			http::Get	request(received_data);

			response = request.execute().str();
		}
		else if (getMethod(received_data) == "POST")
		{
			std::cout << "------------ POST REQUEST ------------" << std::endl;
			http::Post	request(received_data);

			response = request.execute().str();
			//std::cout << "worked" << std::endl;
		}
		else
		{
			response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
        }

        send(client_socket, response.c_str(), response.length(), 0);
        //close(client_socket);
    }

    close(server_socket);
    return 0;
}
