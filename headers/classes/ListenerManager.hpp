/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenerManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:37:18 by wayden            #+#    #+#             */
/*   Updated: 2025/07/04 00:53:27 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENERMANAGER_HPP
#define LISTENERMANAGER_HPP

#include <vector>
class ListenerManager
{
public:
	ListenerManager();
	ListenerManager(const ListenerManager &other);
	~ListenerManager();
	ListenerManager &operator=(const ListenerManager &other);
	void handlePollEvents();
	std::vector<struct pollfd> getnewClients();
private:
	std::vector<struct pollfd> updatedListeners; // List of listeners that need to be
	std::vector<int> newClients; // List of new clients to be added
};



#endif // LISTENERMANAGER_HPP