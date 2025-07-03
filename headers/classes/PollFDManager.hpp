/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollFDManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 18:32:13 by wayden            #+#    #+#             */
/*   Updated: 2025/07/04 00:57:32 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLLFDMANAGER_HPP
#define POLLFDMANAGER_HPP

#include <vector>
#include <map>
#include <poll.h>
#include <unistd.h> // pour close()
#include <cstdio>   // pour perror

class PollFDManager
{
public:
	PollFDManager();
	PollFDManager(const PollFDManager &other);
	~PollFDManager();
	PollFDManager &operator=(const PollFDManager &other);
	
	void addListenerFD(int fd, short events);
	void addClientFD(int fd, short events);
	void addClientFD(std::vector<int> clients_fd, short events); 
	void removeFD(int fd);
	void OnUpdateFinish();
	void modifyFD(int fd, short events);
	int poll(int timeout = -1);
	std::vector<struct pollfd> getFds() const;
	std::vector<struct pollfd> getUpdListenerFds() const;
	std::vector<struct pollfd> getUpdClientFds() const;
	// Closes all file descriptors and clears the list
private:
	void clearFds();
	void closeFds();
	std::vector<struct pollfd> fds;//total list of file descriptors
	std::vector<struct pollfd> upd_listeners; // List of listener that need to be updated
	std::vector<struct pollfd> upd_clients; // List of clients that need to be updated
	int _listenersCount = 0; // Count of listener file descriptors
};

#endif // POLLFDMANAGER_HPP