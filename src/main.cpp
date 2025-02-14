/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmoussou <mmoussou@student.42angouleme.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 15:45:07 by mmoussou          #+#    #+#             */
/*   Updated: 2025/02/12 10:02:11 by mmoussou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <webserv.hpp>
#include <requests/default.hpp>

#define PORT 8080
#define BUFFER_SIZE 4096

int server_socket;

void	close_socket(int signal)
{
	close(server_socket);
	exit(signal);
}

int main() {
	// handle ctrl-C to close server socket
	if (signal(SIGINT, close_socket) == SIG_ERR) {
        std::cerr << "Error registering signal handler!" << std::endl;
        return 1;
    }

    // create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Failed to create socket" << std::endl;
        return 1;
    }

    // prepare the server address
    sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // bind the socket to the address
    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }

    // listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        // accept an incoming connection
        sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        // receive the HTTP request
        char buffer[BUFFER_SIZE];
        std::memset(buffer, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received == -1) {
            std::cerr << "Failed to receive request" << std::endl;
            close(client_socket);
            continue;
        }

        // parse the request
        std::string received_data(buffer, bytes_received);
		//HttpRequest request = parseRequest(received_data);
		http::Get	request(received_data);

        std::cout << "Received " << request.getMethod() << " request for " << request.getTarget() << std::endl;

        // handle the request
		std::string	response;
        if (request.getMethod() == "GET")
		{
			response = request.execute().str();
        }
		else
		{
			response = "HTTP/1.1 501 Not Implemented\r\nContent-Type: text/html\r\n\r\n<html><body><h1>501 Not Implemented</h1></body></html>";
        }

        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
    }

    close(server_socket);
    return 0;
}
