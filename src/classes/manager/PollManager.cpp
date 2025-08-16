/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 22:26:23 by wayden            #+#    #+#             */
/*   Updated: 2025/08/14 09:03:12 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/PollFDManager.hpp"


PollFDManager::PollFDManager()
    : _listenersCount(0)
{}

// Copy constructor
PollFDManager::PollFDManager(const PollFDManager &other)
    : fds(other.fds),
      upd_listeners(other.upd_listeners),
      upd_clients(other.upd_clients),
      _listenersCount(other._listenersCount)
{}

PollFDManager::~PollFDManager() {
	clearFds();
}

PollFDManager &PollFDManager::operator=(const PollFDManager &other)
{
    if (this != &other)
    {
        fds = other.fds;
        upd_listeners = other.upd_listeners;
        upd_clients = other.upd_clients;
        _listenersCount = other._listenersCount;
    }
    return *this;
}


std::vector<struct pollfd> PollFDManager::getUpdatedListeners() const {
	return upd_listeners;
}

std::vector<struct pollfd> PollFDManager::getUpdatedClients() const {
	return upd_clients;
}

void PollFDManager::addListenerFD(int fd, short events) {
	struct pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	fds.push_back(p);
	_listenersCount++;
}

void PollFDManager::removeFD(int fd) {
	size_t i = 0;
	for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it) {
		if (it->fd == fd) {
			if (i < _listenersCount) {
				_listenersCount--;
			}
			close(it->fd);
			fds.erase(it);
			return;
		}
		i++;
	}
}

void PollFDManager::addClientFD(int fd, short events) {
	struct pollfd p;
	p.fd = fd;
	p.events = events;
	p.revents = 0;
	fds.push_back(p);
}

struct pollfd* PollFDManager::addClientFD(struct pollfd client_fd) {
	fds.push_back(client_fd);
	return &fds[fds.size() - 1];
}

void PollFDManager::OnUpdateFinish() {
	upd_listeners.clear();
	upd_clients.clear();
}

void PollFDManager::Update() {
	int res = ::poll(fds.data(), fds.size(), -1);
	if (res < 0) {
		perror("poll");
		//placeholder for error handling
	}
	for(size_t i = 0; i < fds.size(); ++i) {
		if(fds[i].revents & POLLHUP || fds[i].revents & POLLERR || fds[i].revents & POLLNVAL || fds[i].revents & POLLREMOVE) 
		{
			std::cout << "POLLERR or POLLHUP" << std::endl;
			removeFD(fds[i].fd);
			i--;
			continue;
		}
		else if (fds[i].revents && i < _listenersCount) {
			upd_listeners.push_back(fds[i]);
		} else if (fds[i].revents) {
			upd_clients.push_back(fds[i]);
		}
		
		fds[i].revents = 0; // Reset revents after processing
	}
	LogManager::logInfo(toString());
}

void PollFDManager::setEvent(int fd, short events) {
	for (size_t i = 0; i < fds.size(); ++i) {
		if (fds[i].fd == fd) {
			fds[i].events = events;
			break;
		}
	}
}

std::vector<struct pollfd> PollFDManager::getFds() const {
	return fds;
}

void PollFDManager::clearFds() {
	for (size_t i = 0; i < fds.size(); ++i) {
		if (fds[i].fd >= 0) {
			close(fds[i].fd);
		}
	}
	fds.clear();
	upd_listeners.clear();
    upd_clients.clear();
	_listenersCount = 0;
}

std::string pollEventsToString(short ev) {
    std::ostringstream oss;
    bool first = true;
    // Chaque test ajoute le nom du flag correspondant
    if (ev & POLLIN)     { if (!first) oss << "|"; oss << "POLLIN"; first = false; }
    if (ev & POLLPRI)    { if (!first) oss << "|"; oss << "POLLPRI"; first = false; }
    if (ev & POLLOUT)    { if (!first) oss << "|"; oss << "POLLOUT"; first = false; }
    if (ev & POLLERR)    { if (!first) oss << "|"; oss << "POLLERR"; first = false; }
    if (ev & POLLHUP)    { if (!first) oss << "|"; oss << "POLLHUP"; first = false; }
    if (ev & POLLNVAL)   { if (!first) oss << "|"; oss << "POLLNVAL"; first = false; }
    if (first) oss << "0"; // aucun flag actif
    return oss.str();
}

std::string PollFDManager::toString() const {
        std::ostringstream oss;

        oss << "=== Class State ===\n";
        oss << "Listeners count: " << _listenersCount << "\n";

        oss << "\n-- fds (" << fds.size() << ") --\n";
        for (size_t i = 0; i < fds.size(); ++i) {
            oss << "  [" << i << "] fd=" << fds[i].fd
                << " events=" << pollEventsToString(fds[i].events)
                << " revents=" << pollEventsToString(fds[i].revents) << "\n";
        }

        oss << "\n-- upd_listeners (" << upd_listeners.size() << ") --\n";
        for (size_t i = 0; i < upd_listeners.size(); ++i) {
            oss << "  [" << i << "] fd=" << upd_listeners[i].fd
                << " events=" << pollEventsToString(upd_listeners[i].events)
                << " revents=" << pollEventsToString(upd_listeners[i].revents) << "\n";
        }

        oss << "\n-- upd_clients (" << upd_clients.size() << ") --\n";
        for (size_t i = 0; i < upd_clients.size(); ++i) {
            oss << "  [" << i << "] fd=" << upd_clients[i].fd
                << " events=" << pollEventsToString(upd_clients[i].events)
                << " revents=" << pollEventsToString(upd_clients[i].revents) << "\n";
        }

        return oss.str();
}
// Placeholder for private members to store poll file descriptors

