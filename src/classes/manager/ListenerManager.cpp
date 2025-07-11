/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenerManager.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:41:17 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:38:16 by wayden           ###   ########.fr       */
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

void ListenerManager::Update(const std::vector<struct pollfd>& fds) {
	// Iterate through the fds and handle events for listeners

	for (size_t i = 0; i < fds.size(); ++i) {
		if (fds[i].revents & POLLIN) {
			struct sockaddr_storage their_addr;
			socklen_t addr_size;
			addr_size = sizeof(their_addr);
			 
			int fd = accept(fds[i].fd,  (struct sockaddr *)&their_addr, &addr_size);
			if (fd == -1) {
				// replace with logging error
				continue;
			}
			fcntl(fd, F_SETFL, O_NONBLOCK);
			newClient nc;
            nc.pfd.fd = fd;
            nc.pfd.events = POLLIN;
            nc.pfd.revents = 0;
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

