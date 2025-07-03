/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 22:26:23 by wayden            #+#    #+#             */
/*   Updated: 2025/07/04 00:18:13 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/PollFDManager.hpp"

PollFDManager::PollFDManager() {}
PollFDManager::PollFDManager(const PollFDManager &other) : fds(other.fds) {}
PollFDManager::~PollFDManager() {
	clearFds();
}
PollFDManager &PollFDManager::operator=(const PollFDManager &other) {
	if (this != &other) {
		fds = other.fds;
	}
	return *this;
}

void PollFDManager::addListenerFD(int fd, short events) {
	fds.push_back({fd, events, 0});
	_listenersCount++;
}

void PollFDManager::removeFD(int fd) {
	int i = 0;
	for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it) {
		if (it->fd == fd) {
			if (i < _listenersCount) {
				_listenersCount--;
			}
			fds.erase(it);
			return;
		}
		i++;
	}
}

void PollFDManager::addClientFD(int fd, short events) {
	fds.push_back({fd, events, 0});
}

void PollFDManager::OnUpdateFinish() {
	upd_listeners.clear();
	upd_clients.clear();
}

void PollFDManager::modifyFD(int fd, short events) {

}

int PollFDManager::poll(int timeout) {
	int res = ::poll(fds.data(), fds.size(), timeout);
	if (res < 0) {
		perror("poll");
		//placeholder for error handling
	}
	for(int i = 0; i < fds.size(); ++i) {
		if (fds[i].revents && i < _listenersCount) {
			upd_listeners.push_back(fds[i]);
		} else if (fds[i].revents) {
			upd_clients.push_back(fds[i]);
		}
		fds[i].revents = 0; // Reset revents after processing
	}
}

std::vector<struct pollfd> PollFDManager::getFds() const {
	return fds;
}

void PollFDManager::clearFds() {
		for (int i = 0; i < fds.size(); ++i) {
		if (fds[i].fd >= 0) {
			close(fds[i].fd);
		}
	}
	fds.clear();
	_listenersCount = 0;
}

// Placeholder for private members to store poll file descriptors