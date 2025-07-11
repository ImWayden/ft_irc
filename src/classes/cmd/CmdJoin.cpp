/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 00:15:42 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:09:19 by wayden           ###   ########.fr       */
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
bool CmdJoin::isValidChannelName(std::string name)
{
	//
	// check if the channel name is valid
	//
}


std::vector<std::string> CmdJoin::split(const std::string& s, char delimiter) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t end;

	while ((end = s.find(delimiter, start)) != std::string::npos) {
		result.push_back(s.substr(start, end - start));
		start = end + 1;
	}
	result.push_back(s.substr(start));
	return result;
}

std::map<std::string, std::string> CmdJoin::getChannelListFromData(const CommandData& cmd)
{
	std::map<std::string, std::string> channelList;

	std::vector<std::string> names = split(cmd.args[0], ',');
	std::vector<std::string> keys = split(cmd.args[1], ',');

	for (size_t i = 0; i < names.size(); ++i) {
		std::string name = names[i];
		std::string key = (i < keys.size()) ? keys[i] : "";
		if(!isValidChannelName(name))
		{
			//ERR_NOSUCHCHANNEL (403)
			cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOSUCHCHANNEL, ERRSTRING_NOSUCHCHANNEL(name)));
			continue;
		}
		else
			channelList.insert(std::make_pair(name, key));
	}
	return channelList;
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
		_channelManager->removeClientFromAllChannels(cmd.client);
		// probably more to do like signaling to channels a client as left.
		return;
	}
	std::string keysArg = (cmd.args.size() > 1) ? cmd.args[1] : "";
	std::map<std::string, std::string> channelList = getChannelListFromData(cmd);

	for (std::map<std::string, std::string>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	{
		const std::string& channelName = it->first;
		const std::string& key = it->second;

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
			// ERR_BADCHANNELKEY (475)
			if (channel->hasKey() && channel->getKey() != key) {
				cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_BADCHANNELKEY, ERRSTRING_BADCHANNELKEY(channelName)));
				continue;
			}
			// ERR_INVITEONLYCHAN (473)
			if (channel->isInviteOnly() && !channel->isInvited(cmd.client)) {
				cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_INVITEONLYCHAN, ERRSTRING_INVITEONLYCHAN(channelName)));
				continue;
			}
			// ERR_CHANNELISFULL (471)
			if (channel->isFull()) {
				cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_CHANNELISFULL, ERRSTRING_CHANNELISFULL(channelName)));
				continue;
			}
			// ERR_BANNEDFROMCHAN (474)
			if (channel->isBanned(cmd.client)) {
				cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_BANNEDFROMCHAN, ERRSTRING_BANNEDFROMCHAN(channelName)));
				continue;
			}
		}
		//this part can be done in the channel Directly
		channel->addClient(cmd.client);
		cmd.client->joinChannel(channelName);

		// JOIN message (sent to all members)
		std::string joinMsg = MessageMaker::MessageGenerator(cmd, true, 0, channelName);
		channel->broadcast(joinMsg, cmd.client);

		// RPL_TOPIC (332) ou "no topic" (331)
		if (!channel->getTopic().empty()) {
			cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, 332,
				cmd.client->getNickname() + " " + channelName + " :" + channel->getTopic()));
		} else {
			cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, 331,
				cmd.client->getNickname() + " " + channelName + " :No topic is set"));
		}
	}
}

//TODO : channel isFull isBanned(client) isInviteOnly IsInvited(client) hasKey setKey(std::string) getTopic setTopic(std::string) 
