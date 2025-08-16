/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 01:48:45 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 15:12:22 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/ChannelManager.hpp"


ChannelManager::ChannelManager() : _channels() {}

ChannelManager::ChannelManager(const ChannelManager &other) {
	_channels = other._channels;
}

ChannelManager::~ChannelManager() {}

ChannelManager &ChannelManager::operator=(const ChannelManager &other) {
	if (this != &other) {
		_channels = other._channels;
	}
	return *this;
}

//probably gonna get deleted since each Channels "manages" its own clients
void ChannelManager::leaveChannel(const std::string &name, Client* client) {
	_channels[name].removeClient(client); //do i need to check if it exists?
}

void ChannelManager::sendMessageToChannel(const std::string &name, ServerMessage_t message, Client* sender) {
	_channels[name].broadcast(message, sender);
}


bool ChannelManager::isChannelExisting(const std::string &name) const {
	return (_channels.find(name) != _channels.end());
}

Channel* ChannelManager::createChannel(const std::string &name) {
	_channels[name] = Channel(name);
	return &(_channels[name]);
}

//do not erase the entry in the map and try to use the pointer to acces the erased channel
Channel* ChannelManager::getChannel(const std::string &name){
	std::map<std::string, Channel>::iterator it = _channels.find(name);
	if (it != _channels.end())
		return &(it->second);
	return NULL;
}

void ChannelManager::removeClientFromChannel(const std::string &name, Client *client) {
	_channels[name].removeClient(client);
}

void ChannelManager::removeClientFromChannels(std::vector<std::string> &names, Client *client) {
	std::vector<std::string>::iterator it = names.begin();
	while (it != names.end()) {
		_channels[*it].removeClient(client);
		it++;
	}
}

void ChannelManager::removeClientFromChannels(std::set<std::string> &names, Client *client, std::string message) {
	std::set<std::string>::iterator it = names.begin();
	while (it != names.end()) {
		_channels[*it].removeClient(client);
		if (!message.empty())
			_channels[*it].broadcast(message, client);
		it++;
	}
}

void ChannelManager::removeClientFromAllChannels(Client* client, std::string message) {
	std::map<std::string, Channel>::iterator it = _channels.begin();
	while (it != _channels.end()) {
		it->second.broadcast(MessageMaker::MessageGenerator(client->getPrefix(), "PART", !message.empty() ? message : ":disconnected from the channel"), client);
		it->second.removeClient(client);
		it++;
	}
}

/*
class ChannelManager
{
public:
	ChannelManager();
	ChannelManager(const ChannelManager &other);
	~ChannelManager();
	ChannelManager &operator=(const ChannelManager &other);	

	void leaveChannel(const std::string &name, int clientFd);
	void sendMessageToChannel(const std::string &name, ServerMessage_t message);
	//void listChannels() const;

	bool isChannelExisting(const std::string &name) const;
	void createChannel(const std::string &name);
	
	Channel* getChannel(const std::string &name) const;
	
	void removeClientFromChannel(const std::string &name, int clientFd);
	void removeClientFromChannels(std::vector<std::string>, int clientFd);
	void removeClientFromAllChannels(Client* client);
private:
	std::map<std::string, Channel *> channels; // Map to store channels by name
	// Placeholder for channel management methods
}
*/