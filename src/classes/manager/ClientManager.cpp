/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:21:09 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 21:22:25 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ClientManager.hpp"

#include <poll.h>

ClientManager::ClientManager(IPollControl* pollControl) : _pollControl(pollControl) {}
ClientManager::ClientManager(const ClientManager &other) : _clients(other._clients)/*, _clientsfd(other._clientsfd)*/ {}
ClientManager::~ClientManager() {}

ClientManager &ClientManager::operator=(const ClientManager &other) {
	if (this != &other) {
		//_clientsfd = other._clientsfd;
		_clients = other._clients;
	}
	return *this;
}

void ClientManager::addClient(int fd) {
	_clients[fd] = Client(fd);
}

void ClientManager::addClient(int fd, struct sockaddr_storage addr) {
	_clients[fd] = Client(fd, _pollControl);
	_clients[fd].setAddr(addr);
}


void ClientManager::removeClient(int fd) {
	_clients.erase(fd);
}

void ClientManager::Update(const std::vector<struct pollfd> fds) {
	for (size_t i = 0; i < fds.size(); ++i) {
		int fd = fds[i].fd;
		if (_clients.find(fd) == _clients.end()) 
		{
			//warning client not found
			continue;
		}
		Client &client = _clients[fd];
		if (fds[i].revents & POLLIN && client.receiveMessages()) 
			_upd_clients.push_back(&client);
		if (fds[i].revents & POLLOUT)
			client.sendMessages();
		if (client.getQuitStatus() == QUITTING_DONE)
			_quitting_clients.push_back(&client);
	}
}


Client* ClientManager::getClientByNickname(std::string nickname) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second.getNickname() == nickname)
			return &it->second;
	return NULL;
}

//not optimized i should have a set of clients nicknames ready to use, but that signify adding nicknames during the nick command, annoying asf
std::set<std::string> ClientManager::getNicknames() {
	std::set<std::string> nicknames;
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		nicknames.insert(it->second.getNickname());
	return nicknames;
}

void ClientManager::OnUpdateFinish() {
	for(std::vector<Client*>::iterator it = _upd_clients.begin(); it != _upd_clients.end(); ++it)
		(*it)->getMsgReceived()->clear();
	_upd_clients.clear();
	_quitting_clients.clear();
}