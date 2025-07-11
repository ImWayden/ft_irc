/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:55:41 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:37:54 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ServerManager.hpp"


ServerManager::ServerManager() : _portManager(), _passwordManager(), _pollFDManager(), _clientManager(&_pollFDManager), _channelManager() {}

ServerManager::ServerManager(const std::string &port, const std::string &password)
	: _portManager(port), _passwordManager(password), _pollFDManager(), _clientManager(&_pollFDManager), _channelManager() {}

ServerManager::ServerManager(const ServerManager &other)
	: _portManager(other._portManager), _passwordManager(other._passwordManager), _pollFDManager(other._pollFDManager), _clientManager(other._clientManager), _channelManager(other._channelManager) {}

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
		_pollFDManager.addListenerFD(server_fd, POLLIN);
		fcntl(server_fd, F_SETFL, O_NONBLOCK);
		if (listen(server_fd, 5) == -1) {
			perror("listen");
			close(server_fd);
			_pollFDManager.removeFD(server_fd);
		}
	}
	freeaddrinfo(res);
	if(_pollFDManager.getFds().empty()) {
		std::cerr << "Failed to create server socket" << std::endl;
		return -1;
	}
}

int ServerManager::Update()
{
	_pollFDManager.Update();
	_listenerManager.Update(_pollFDManager.getUpdatedListeners());
	_clientManager.Update(_pollFDManager.getUpdatedClients());
	_commandManager.Update(_clientManager.getUpdClients());
	onUpdateFinish();
}


void ServerManager::onUpdateFinish()
{
	handleNewClients(_listenerManager.getnewClients());
	handleQuittingClients(_clientManager.getQuittingClients());
	_listenerManager.OnUpdateFinish();
	_clientManager.OnUpdateFinish();
	//_commandManager.OnUpdateFinish();
	_pollFDManager.OnUpdateFinish();
}

void ServerManager::handleNewClients(std::vector<newClient> newclients) {
	for (int i = 0; i < newclients.size(); ++i) {
		newClient &client = newclients[i];
		_pollFDManager.addClientFD(client.client_fd);
		_clientManager.addClient(client.client_fd.fd, client.addr);
	}
}

void ServerManager::handleQuittingClients(std::vector<Client *> quittingclients) {
	for (int i = 0; i < quittingclients.size(); ++i) {
		int fd = quittingclients[i]->getFd();
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
