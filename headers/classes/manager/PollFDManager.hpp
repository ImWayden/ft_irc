/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollFDManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:32:13 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 18:43:10 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLFDMANAGER_HPP
#define POLLFDMANAGER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h> // pour close()
#include <cstdio>   // pour perror
#include <sstream>

#include "interfaces/IPollControl.hpp"
#include "LogManager.hpp"

class PollFDManager : public IPollControl
{
public:
	PollFDManager();
	PollFDManager(const PollFDManager &other);
	~PollFDManager();
	PollFDManager &operator=(const PollFDManager &other);
	
	void addListenerFD(int fd, short events);
	void addClientFD(int fd, short events);
	struct pollfd* addClientFD(struct pollfd client_fd);
	
	void removeFD(int fd);
	void OnUpdateFinish();
	void Update();

	virtual void setEvent(int fd, short events);
	std::vector<struct pollfd> getFds() const;
	std::vector<struct pollfd> getUpdatedListeners() const;
	std::vector<struct pollfd> getUpdatedClients() const;
	void clearFds();
private:
	std::string toString() const;
	std::vector<struct pollfd> fds;
	std::vector<struct pollfd> upd_listeners;
	std::vector<struct pollfd> upd_clients;
	size_t _listenersCount; 
};

#endif // POLLFDMANAGER_HPP