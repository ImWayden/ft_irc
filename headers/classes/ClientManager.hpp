/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:57:52 by wayden            #+#    #+#             */
/*   Updated: 2025/07/04 00:59:35 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include <string>
#include <vector>
#include <map>

#include "Client.hpp" // Assuming Client is a class that holds client-related data

class ClientManager
{
public:
	ClientManager();
	ClientManager(const ClientManager &other);
	~ClientManager();
	ClientManager &operator=(const ClientManager &other);

	// Placeholder for client management methods
	void addClient(int fd);
	void removeClient(int fd);
	void handlePollEvents();
	void sendMessageToClient(int fd, const std::string &message);
private:
	std::vector<int> clients; // List of client file descriptors
	std::map<int, Client> clientData; // Map to store client data (e.g., nicknames, channels)
	// Placeholder for other client-related data structures
};



#endif // CLIENTMANAGER_HPP