/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:55:41 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:03:14 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ServerManager.hpp"
#include <iostream>

ServerManager::ServerManager() : _passwordManager(), _portManager(), _pollFDManager(), _clientManager(&_pollFDManager), _commandManager(), _channelManager() {}

ServerManager::ServerManager(const std::string &port, const std::string &password)
	:  _passwordManager(password), _portManager(port), _pollFDManager(), _clientManager(&_pollFDManager), _commandManager(this), _channelManager() {}

ServerManager::ServerManager(const ServerManager &other)
	: _passwordManager(other._passwordManager), _portManager(other._portManager), _pollFDManager(other._pollFDManager), _clientManager(other._clientManager), _channelManager(other._channelManager) {}

ServerManager::~ServerManager() {}

int ServerManager::Init()
{
	std::vector<int> sockets;
	if(!_portManager.getStatus())
		return -1;
	else if(!_passwordManager.getStatus())
		return -1;
	sockets = _listenerManager.Init(_portManager.getPortString());
	if (sockets.empty())
		return -1;
	for (std::vector<int>::iterator it = sockets.begin(); it != sockets.end(); ++it)
	{
	    int fd = *it;
	    _pollFDManager.addListenerFD(fd, POLLIN);
	    fcntl(fd, F_SETFL, O_NONBLOCK);
	}
	return 0;
}

int ServerManager::Update()
{
	LogManager::logServerTech("Update");
	LogManager::logServerTech("PollFDManager Update");
	_pollFDManager.Update();
	_listenerManager.Update(_pollFDManager.getUpdatedListeners());
	_clientManager.Update(_pollFDManager.getUpdatedClients());
	_commandManager.Update(_clientManager.getUpdClients());
	return 0;
}

void ServerManager::onUpdateFinish()
{
	handleNewClients(_listenerManager.getnewClients());
	handleQuittingClients(_clientManager.getQuittingClients());
	_listenerManager.OnUpdateFinish();
	_clientManager.OnUpdateFinish();
	_pollFDManager.OnUpdateFinish();
}

void ServerManager::OnClose() 
{
	_pollFDManager.clearFds();
}

void ServerManager::handleNewClients(std::vector<newClient> newclients) {
	for (size_t i = 0; i < newclients.size(); ++i) {
		newClient &client = newclients[i];
		_pollFDManager.addClientFD(client.client_fd);
		_clientManager.addClient(client.client_fd.fd, client.addr);
	}
}

void ServerManager::handleQuittingClients(std::vector<Client *> quittingclients) {
	for (size_t i = 0; i < quittingclients.size(); ++i) {
		int fd = quittingclients[i]->getFd();
		_channelManager.removeClientFromAllChannels(quittingclients[i]);
		_pollFDManager.removeFD(fd);
		_clientManager.removeClient(fd);
	}
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


PasswordManager &ServerManager::getPasswordManager() {
	return _passwordManager;
}

ClientManager &ServerManager::getClientManager() {
	return _clientManager;
}

ChannelManager &ServerManager::getChannelManager() {
	return _channelManager;
}

PollFDManager &ServerManager::getPollFDManager() {
	return _pollFDManager;
}