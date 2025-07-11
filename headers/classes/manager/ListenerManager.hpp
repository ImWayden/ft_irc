/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenerManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:37:18 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:34:39 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENERMANAGER_HPP
#define LISTENERMANAGER_HPP

#include "interfaces/IPollControl.hpp"
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include <fcntl.h> 

struct newClient {
	struct pollfd pfd; // File descriptor for the client socket
	struct sockaddr_storage addr; // Address of the client
};

class ListenerManager
{
public:
	ListenerManager();
	ListenerManager(const ListenerManager &other);
	~ListenerManager();
	ListenerManager &operator=(const ListenerManager &other);
	
	void Update(const std::vector<struct pollfd>& fds);
	void OnUpdateFinish();
	
	std::vector<newClient> getnewClients();
private:
	std::vector<newClient> _newclients; // List of new clients to be added
};




#endif // LISTENERMANAGER_HPP