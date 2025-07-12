/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:57:52 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 21:21:55 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <string>
#include <vector>
#include <map>
#include <set>

#include "classes/data/Client.hpp" // Assuming Client is a class that holds client-related data

/*
	is responsible for adding client and updating client having received a specific event

*/
class ClientManager
{
public:
	ClientManager(IPollControl* pollControl);
	ClientManager(const ClientManager &other);
	~ClientManager();
	ClientManager &operator=(const ClientManager &other);
	std::set<std::string> ClientManager::getNicknames();
	// Placeholder for client management methods
	void addClient(int fd);
	void addClient(int fd, struct sockaddr_storage addr);

	void removeClient(int fd);
	
	void Update(std::vector<struct pollfd> fds);
	void OnUpdateFinish();

	Client* getClientByNickname(std::string nickname);
	std::vector<Client*> getUpdClients();
	std::vector<Client*> getQuittingClients();
private:
	//std::vector<int> _clientsfd; // List of client file descriptors useless ??
	std::map<int, Client> _clients; // Map to store client data (e.g., nicknames, channels)
	std::vector<Client* > _upd_clients; // List of clients to be updated <Client*, std::vector<ClientMessage_t>*>
	std::vector<Client* > _quitting_clients;
	IPollControl* _pollControl;
	// Placeholder for other client-related data structures
};



#endif // CLIENTMANAGER_HPP