/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:55:41 by wayden            #+#    #+#             */
/*   Updated: 2025/07/04 00:56:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/ServerManager.hpp"
#include <iostream>
#include <sys/types.h>      // types de base (ex: ssize_t)
#include <sys/socket.h>     // socket(), bind(), listen(), accept()
#include <netinet/in.h>     // sockaddr_in (IPv4)
#include <arpa/inet.h>      // inet_pton(), inet_ntop()
#include <unistd.h>         // close()
#include <netdb.h>          // getaddrinfo(), freeaddrinfo()
#include <cstring>          // memset()
#include <fcntl.h> 

ServerManager::ServerManager() : _portManager(), _passwordManager(), _pollFDManager() {}

ServerManager::ServerManager(const std::string &port, const std::string &password)
	: _portManager(port), _passwordManager(password), _pollFDManager() {}

ServerManager::ServerManager(const ServerManager &other)
	: _portManager(other._portManager), _passwordManager(other._passwordManager), _pollFDManager(other._pollFDManager) {}

ServerManager::~ServerManager() {}

int ServerManager::Init()
{
	int server_fd;
	struct sockaddr_in serv_addr, cli_addr;
	struct addrinfo hints, *res, *p;

	//prepare the hints structure which will be used by getaddrinfo
	//to specify the type of socket we want to create
	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 and IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE; // Use my IP
	
	if (getaddrinfo(NULL, _portManager.getPortString().c_str(), &hints, &res) != 0) {
		std::cerr << "getaddrinfo error" << std::endl;
		return;
	}
	for (p = res; p != NULL; p = p->ai_next) {
		server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (server_fd == -1) {
			perror("socket");//replace with warning logging
			continue;
		}
		if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_fd);
			perror("bind"); // replace with logging warning
			continue;
		}
		_pollFDManager.addListenerFD(server_fd, POLLIN); // Add the socket to the poll manager
		fcntl(server_fd, F_SETFL, fcntl(server_fd, F_GETFL, 0) | O_NONBLOCK);
		if (listen(server_fd, 5) == -1) {
			perror("listen"); //replace with logging warning
			close(server_fd);
			_pollFDManager.removeFD(server_fd); // Remove the socket from the poll manager
		}
	}
	freeaddrinfo(res); // free the linked list of addrinfo structure
	if(_pollFDManager.getFds().empty()) {
		std::cerr << "Failed to create server socket" << std::endl;
		return -1;
	}
}

int ServerManager::Update()
{
	
	_pollFDManager.poll(-1); // Poll for events
	std::vector<struct pollfd> upd_listener_fds = _pollFDManager.getUpdListenerFds();
	_listenerManager.handlePollEvents(); // Handle events for listeners
	
	std::vector<struct pollfd> upd_clients_fds = _pollFDManager.getUpdClientFds();
	
	return 0;
}

ServerManager &ServerManager::operator=(const ServerManager &other) {
	if (this != &other) {
		_portManager = other._portManager;
		_passwordManager = other._passwordManager;
	}
	return *this;
}	

const std::string &ServerManager::getPort() const {
	return _portManager.getPortString();
}

const std::string &ServerManager::getPassword() const {
	return _passwordManager.getPassword();
}

// Note: The ServerManager class is designed to manage the server's port and password,