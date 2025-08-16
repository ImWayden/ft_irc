/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ListenerManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:37:18 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:04:17 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENERMANAGER_HPP
#define LISTENERMANAGER_HPP

#include "interfaces/IPollControl.hpp"
#include <vector>
#include <poll.h>
#include <netinet/in.h>
#include <iostream>      // std::cerr, std::cout (si besoin)
#include <cstring>       // std::memset, strerror
#include <string>        // std::string

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "struct/struct.hpp"
#include "LogManager.hpp"
class ListenerManager
{
public:
	ListenerManager();
	ListenerManager(const ListenerManager &other);
	~ListenerManager();
	ListenerManager &operator=(const ListenerManager &other);
	

	std::vector<int> Init(std::string port);
	void Update(const std::vector<struct pollfd>& fds);
	void OnUpdateFinish();
	
	std::vector<newClient> getnewClients();
private:
	std::vector<newClient> _newclients;
};




#endif // LISTENERMANAGER_HPP