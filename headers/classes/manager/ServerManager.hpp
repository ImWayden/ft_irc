/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:49:32 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:26:54 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "PasswordManager.hpp"
#include "PortManager.hpp"
#include "PollFDManager.hpp"
#include "ListenerManager.hpp" 
#include "ClientManager.hpp"
#include "CommandManager.hpp"
#include "ChannelManager.hpp" // Assuming ChannelManager is defined elsewhere

#include <iostream>
#include <sys/types.h>      // types de base (ex: ssize_t)
#include <sys/socket.h>     // socket(), bind(), listen(), accept()
#include <netinet/in.h>     // sockaddr_in (IPv4)
#include <arpa/inet.h>      // inet_pton(), inet_ntop()
#include <unistd.h>         // close()
#include <netdb.h>          // getaddrinfo(), freeaddrinfo()
#include <cstring>          // memset()
#include <fcntl.h>          // fcntl

#include <vector>
class ServerManager
{
public:
	ServerManager();
	ServerManager(const std::string &port, const std::string &password);
	ServerManager(const ServerManager &other);
	~ServerManager();

	int Init(); // Placeholder for initialization logic
	int Update(); // Placeholder for update logic
	int OnAccept(); // Placeholder for accept logic
	
	ServerManager &operator=(const ServerManager &other);
	const std::string &getPort() const;
	const std::string &getPassword() const;
	
	void onUpdateFinish(); // Placeholder for update finish logic
	void dispatchNewClients(std::vector<newClient> newClients); // Handle new clients from listeners

	std::string getPassword();
	PasswordManager &getPasswordManager();
	ClientManager &getClientManager();
	ChannelManager &getChannelManager();
	PollFDManager &getPollFDManager();
	

	
private:
	void handleNewClients(std::vector<newClient> newclients);
	void handleQuittingClients(std::vector<Client *> quittingclients);

	PasswordManager _passwordManager;
	PortManager _portManager;
	PollFDManager _pollFDManager;
	ListenerManager _listenerManager;
	ClientManager _clientManager;
	CommandManager _commandManager;
	ChannelManager _channelManager;
};


#endif // SERVERMANAGER_HPP