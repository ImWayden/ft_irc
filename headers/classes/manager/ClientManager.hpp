/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:57:52 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:04:57 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_HPP
#define CLIENTMANAGER_HPP

#include "LogManager.hpp"

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
	ClientManager();
	ClientManager(IPollControl* pollControl);
	ClientManager(const ClientManager &other);
	~ClientManager();
	ClientManager &operator=(const ClientManager &other);
	std::set<std::string> getNicknames();

	void addClient(int fd);
	void addClient(int fd, struct sockaddr_storage addr);

	void removeClient(int fd);
	
	void Update(std::vector<struct pollfd> fds);
	void OnUpdateFinish();

	Client* getClientByNickname(std::string nickname);
	Client* getClientByUserAndHost(std::string username, std::string hostname);
	Client* getClientByUsername(std::string username);
	std::vector<Client*> getUpdClients();
	std::vector<Client*> getQuittingClients();
private:

	std::map<int, Client> _clients;
	std::vector<Client* > _upd_clients;
	std::vector<Client* > _quitting_clients;
	IPollControl* _pollControl;

};



#endif // CLIENTMANAGER_HPP