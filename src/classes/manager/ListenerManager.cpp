/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenerManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:41:17 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:35:13 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ListenerManager.hpp"

ListenerManager::ListenerManager() {}
ListenerManager::ListenerManager(const ListenerManager &other) : _newclients(other._newclients) {}
ListenerManager::~ListenerManager() {}

ListenerManager &ListenerManager::operator=(const ListenerManager &other) {
	if (this != &other) {
		_newclients = other._newclients;
	}
	return *this;
}

std::vector<int> ListenerManager::Init(std::string port) {
	std::vector<int> sockets;
	int server_fd;
	struct addrinfo hints, *res, *p;

	//prepare the hints structure which will be used by getaddrinfo
	//to specify the type of socket we want to create
	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // IPv4 and IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP
	hints.ai_flags = AI_PASSIVE; // Use my IP
	
	if (getaddrinfo(NULL, port.c_str(), &hints, &res) != 0) {
		LogManager::logError("Failed to get address info");
		return sockets;
	}
	for (p = res; p != NULL; p = p->ai_next) {
		server_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (server_fd == -1) {
			LogManager::logWarning("Failed socket");
			continue;
		}
		if (bind(server_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_fd);
			LogManager::logWarning("Failed to bind server socket");
			continue;
		}
		fcntl(server_fd, F_SETFL, O_NONBLOCK);
		if (listen(server_fd, 5) == -1) {
			LogManager::logWarning("Failed to listen server socket");
			close(server_fd);
			continue;
		}
		sockets.push_back(server_fd);
	}
	freeaddrinfo(res);
	return sockets;
}

void ListenerManager::Update(const std::vector<struct pollfd>& fds) {
	// Iterate through the fds and handle events for listeners
	LogManager::logServerTech("ListenerManager Update");
	for (size_t i = 0; i < fds.size(); ++i) {
		if (fds[i].revents & POLLIN) {
			struct sockaddr_storage their_addr;
			socklen_t addr_size;
			addr_size = sizeof(their_addr);
			 
			int fd = accept(fds[i].fd,  (struct sockaddr *)&their_addr, &addr_size);
			if (fd == -1) {
				LogManager::logWarning("Accept Failed");
				continue;
			}
			fcntl(fd, F_SETFL, O_NONBLOCK);
			newClient nc;
            nc.client_fd.fd = fd;
            nc.client_fd.events = POLLIN;
            nc.client_fd.revents = 0;
            nc.addr = their_addr;
			_newclients.push_back(nc);
		}
	}
}

void ListenerManager::OnUpdateFinish() {
	_newclients.clear();
}

// need to activate pollout only when we have data to send 
std::vector<newClient> ListenerManager::getnewClients() {
	return _newclients;
}

