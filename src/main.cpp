/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 13:27:16 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:43:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/includes.hpp"
#include <iostream>
#include <sys/types.h>      // types de base (ex: ssize_t)
#include <sys/socket.h>     // socket(), bind(), listen(), accept()
#include <netinet/in.h>     // sockaddr_in (IPv4)
#include <arpa/inet.h>      // inet_pton(), inet_ntop()
#include <unistd.h>         // close()
#include <netdb.h>          // getaddrinfo(), freeaddrinfo()
#include <cstring>          // memset()


int main(int argc, char **argv)
{
	if(argc != R_ARGC)
	{
		std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	std::string port = argv[1];
	std::string password = argv[2];

	
	socket();
	std::cout << "Server on port: " << port << std::endl;
	std::cout << "Password: " << password << std::endl;
	return 0;
}

