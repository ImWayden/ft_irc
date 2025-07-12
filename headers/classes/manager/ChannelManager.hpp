/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 20:06:55 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 22:19:29 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMANAGER_HPP
#define CHANNELMANAGER_HPP

#include <string>
#include <map>
#include "classes/data/Channel.hpp"
#include "classes/data/Client.hpp"
#include "ressources/r_numbers.hpp"
#include "ressources/r_strings.hpp"
#include "utils/MessageMaker.hpp"

class ChannelManager
{
public:
	ChannelManager();
	ChannelManager(const ChannelManager &other);
	~ChannelManager();
	ChannelManager &operator=(const ChannelManager &other);	

	void leaveChannel(const std::string &name, Client* client);
	void sendMessageToChannel(const std::string &name, ServerMessage_t message, Client* sender);
	void listChannels() const;

	bool isChannelExisting(const std::string &name) const;
	Channel* createChannel(const std::string &name);
	
	Channel* getChannel(const std::string &name);

	void removeClientFromChannel(const std::string &name, Client *client);
	void removeClientFromChannels(std::vector<std::string> &names, Client *client);
	void removeClientFromChannels(std::set<std::string> &names, Client *client, std::string message = "");
	void removeClientFromAllChannels(Client* client, std::string message = "");

	void addMessageToChannel(const std::string &name, ClientMessage_t message);
private:
	std::map<std::string, Channel> _channels; // Map to store channels by name
	// Placeholder for channel management methods
};

#endif // CHANNELMANAGER_HPP


//TODO : channelManager isChannelExisiting(ChannelName) createChannel(channelName) getChannel(channelName)