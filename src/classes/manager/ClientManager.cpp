/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:21:09 by wayden            #+#    #+#             */
/*   Updated: 2025/08/17 13:08:50 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ClientManager.hpp"

#include <poll.h>

ClientManager::ClientManager() {}
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
	_clients[fd] = Client(fd, _pollControl);
}

void ClientManager::addClient(int fd, struct sockaddr_storage addr) {
	_clients[fd] = Client(fd, _pollControl);
	_clients[fd].setAddr(addr);
	
	LogManager::logInfo("Client added" + _clients[fd].toString());
}


void ClientManager::removeClient(int fd) {
	LogManager::logInfo( "Client Getting Removed" + _clients[fd].toString());
	_clients.erase(fd);
	LogManager::logInfo("Client Removed");
}

void ClientManager::Update(const std::vector<struct pollfd> fds) {
	LogManager::logServerTech("ClientManager Update");
	for (size_t i = 0; i < fds.size(); ++i) {
		int fd = fds[i].fd;
		if (_clients.find(fd) == _clients.end()) 
		{
			LogManager::logWarning("Client Not Found");
			continue;
		}
		Client &client = _clients[fd];
		//LogManager::logServerTech("Client Geting updated" + _clients[fd].toString());
		if (fds[i].revents & POLLIN && client.receiveMessages()) 
			_upd_clients.push_back(&client);
		if (fds[i].revents & POLLOUT)
			client.sendMessages();
		if (client.getQuitStatus() & QUITTING_DONE || fds[i].revents & POLLHUP || fds[i].revents & POLLERR || fds[i].revents & POLLNVAL || fds[i].revents & POLLREMOVE)
		{
			if(!(client.getQuitStatus() & QUITTING_DONE))
				client.setQuitStatus(QUITTING_DONE);
			_quitting_clients.push_back(&client);
			
		}

	}
}


Client* ClientManager::getClientByNickname(std::string nickname) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second.getNickname() == nickname)
			return &it->second;
	return NULL;
}

Client* ClientManager::getClientByUserAndHost(std::string username, std::string hostname, bool &found_multiple)
{
	Client* client = NULL;
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second.getUsername() == username && it->second.getHostname() == hostname)
		{
			if(client != NULL)
			{
				found_multiple = true;
				client = NULL;
				break;
			}
			client = &it->second;
		}
	return client;
}

Client* ClientManager::getClientByUsername(std::string username, bool &found_multiple)
{
	Client* client = NULL;
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		if (it->second.getUsername() == username)
		{
			if(client != NULL)
			{
				found_multiple = true;
				client = NULL;
				break;
			}
			client = &it->second;
		}
	return client;
}

//not optimized i should have a set of clients nicknames ready to use, but that signify adding nicknames during the nick command, annoying asf
std::set<std::string> ClientManager::getNicknames() {
	std::set<std::string> nicknames;
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
		nicknames.insert(it->second.getNickname());
	return nicknames;
}

std::vector<Client*> ClientManager::getUpdClients() {
	return _upd_clients;
}

std::vector<Client*> ClientManager::getQuittingClients() {
	return _quitting_clients;
}

void ClientManager::OnUpdateFinish() {
	for(std::vector<Client*>::iterator it = _upd_clients.begin(); it != _upd_clients.end(); ++it)
		(*it)->getMsgReceived()->clear();
	_upd_clients.clear();
	_quitting_clients.clear();
	LogManager::logServerTech("Client Manager OnUpdateFinish");
}