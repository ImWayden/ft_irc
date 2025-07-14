/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 00:15:42 by wayden            #+#    #+#             */
/*   Updated: 2025/07/13 13:42:24 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "classes/cmd/CmdJoin.hpp"

/*
	If a JOIN is successful, the user is then sent the channel's topic
	(using RPL_TOPIC) and the list of users who are on the channel (using
	RPL_NAMREPLY), which must include the user joining.

	
*/

/*	
**	channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
**	[ ":" chanstring ]
**
**	chanstring =  %x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B
**	chanstring =/ %x2D-39 / %x3B-FF
**	; any octet except NUL, BELL, CR, LF, " ", "," and ":"
**
**	channelid  = 5( %x41-5A / digit )   ; 5( A-Z / 0-9 )
*/

void CmdJoin::tryJoinChannel(const std::string& channelName, const std::string& key, Client* client, const CommandData& cmd) {
		Channel* channel = NULL; //no nullptr in cpp98
		bool isNew = false;
		
		channel = _channelManager->getChannel(channelName);
		if (channel == NULL) {
			channel = _channelManager->createChannel(channelName);
			channel->setKey(key); // Set only if first time
			isNew = true;
		}
		else
		{
			if (channel->hasKey() && channel->getKey() != key)
				return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_BADCHANNELKEY, ERRSTRING_BADCHANNELKEY(channelName)));
			if (channel->isInviteOnly() && !channel->isInvited(client))
				return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_INVITEONLYCHAN, ERRSTRING_INVITEONLYCHAN(channelName)));
			if (channel->isFull())
				return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_CHANNELISFULL, ERRSTRING_CHANNELISFULL(channelName)));
			if (channel->isBanned(client))
				return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_BANNEDFROMCHAN, ERRSTRING_BANNEDFROMCHAN(channelName)));
		}
		//this part can be done in the channel Directly
		channel->addClient(cmd.client);
		cmd.client->joinChannel(channelName);

		std::string joinMsg = MessageMaker::MessageGenerator(cmd, true, 0, channelName);
		channel->broadcast(joinMsg, cmd.client);

		// RPL_TOPIC (332) ou "no topic" (331)
		if (!channel->getTopic().empty()) 
			return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, RPLCODE_TOPIC, RPLSTRING_TOPIC(channelName, channel->getTopic())));
		else 
			return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, RPLCODE_NOTOPIC, RPLSTRING_NOTOPIC(channelName)));
}



//i could add a feedback struct to let the command manager know what happens with the command
// it could contain a bool that indicate if a message has been added
// it could indicate if the command has been executed
void CmdJoin::execute(const CommandData& cmd)
{
	// ERR_NEEDMOREPARAMS (461)
	if (cmd.args.empty()) {
		cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
		return;
	}
	if (cmd.args[0] == "0") {
		std::string message = MessageMaker::MessageGenerator(cmd, true, 0, ":disconnected from the server");
		_channelManager->removeClientFromAllChannels(cmd.client, message);
		// probably more to do like signaling to channels a client as left.
		return;
	}
	std::string keysArg = (cmd.args.size() > 1) ? cmd.args[1] : "";
	std::map<std::string, std::string> channelList = getChannelListFromData(cmd);

	for (std::map<std::string, std::string>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	{
		const std::string& channelName = it->first;
		const std::string& key = it->second;
		tryJoinChannel(channelName, key, cmd.client, cmd);
	}
}

//TODO : channel isFull isBanned(client) isInviteOnly IsInvited(client) hasKey setKey(std::string) getTopic setTopic(std::string) 
