/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:49:32 by wayden            #+#    #+#             */
/*   Updated: 2025/07/04 00:56:08 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include "PasswordManager.hpp"
#include "PortManager.hpp"
#include "PollFDManager.hpp"
#include "ListenerManager.hpp" 

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
private:
	PasswordManager _passwordManager;
	PortManager _portManager;
	PollFDManager _pollFDManager;
	ListenerManager _listenerManager;
};


#endif // SERVERMANAGER_HPP