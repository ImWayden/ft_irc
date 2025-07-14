/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:07:18 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 00:01:29 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include <string>
#include <set>
#include <iostream>


#define MAX_CLIENTS 50 //arbitrary value need to check the documentation

class Channel {
public:	Channel(const std::string &name);
Channel(const std::string &name, const std::string &key);
	Channel(const Channel &other);
	~Channel();
	Channel &operator=(const Channel &other);
	void addClient(int clientFd);
	void addClient(Client *client);

	void addOperator(Client *client);
	void removeOperator(Client *client);
	
	void removeClient(Client *clientFd);
	void broadcast(const std::string &message, Client* senderFd);
	//void listClients() const;

	bool isFull() const;
	bool isBanned(Client *client) const;
	bool isInviteOnly() const;
	bool isInvited(Client *client) const;
	bool isOperator(Client *client) const;
	bool isTopicProtected() const;
	
	bool hasKey() const;

	void setInviteOnly(bool inviteOnly);
	void setKey(const std::string &key);
	void setName(const std::string &name);
	void setTopic(const std::string &topic);
	void setLimit(int limit);
	void setTopicProtected(bool isProtected);

	std::string getKey() const;
	std::string getName() const;
	std::string getTopic() const;

	void Ban(Client *client);
	void Invite(Client *client);
private:
	bool _isFull;
	bool _isInviteOnly;
	bool _hasKey;
	bool _isTopicProtected;
	int _maxClients;
	std::string _key;
	std::string _name; // Name of the channel
	std::string _topic;
	std::set<Client *> _invited;
	std::set<Client *> _bannneds;
 	std::set<Client *> _clients; // Set of client file descriptors in the channel
	std::set<Client *> _operators; // Set of operator file descriptors in the channel
};

#endif // CHANNEL_HPP

/*
TODO : 
	check documentation about maxclient in a channel
	check if max client is modddable on runtime
	check if the client sends the msg to himself too

*/