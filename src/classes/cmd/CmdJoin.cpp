/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 00:15:42 by wayden            #+#    #+#             */
/*   Updated: 2025/08/17 18:51:51 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "classes/cmd/CmdJoin.hpp"


//TODO Cannonical form

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

CmdJoin::CmdJoin(ChannelManager& channelManager) : _channelManager(&channelManager) {}

CmdJoin::CmdJoin() {}

CmdJoin::~CmdJoin() {}

CmdJoin::CmdJoin(const CmdJoin &other) {
	*this = other;
}

CmdJoin &CmdJoin::operator=(const CmdJoin &other) {
	_channelManager = other._channelManager;
	return (*this);
}


void CmdJoin::tryJoinChannel(const std::string& channelName, const std::string& key, Client* client, const CommandData& cmd) {
	Channel* channel = NULL; //no nullptr in cpp98
	
	channel = _channelManager->getChannel(channelName);
	if (channel == NULL) {
		channel = _channelManager->createChannel(channelName);
		channel->addOperator(client);
		channel->setKey(key); // Set only if first time
	}
	else
	{
		if (channel->hasKey() && channel->getKey() != key)
			return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_BADCHANNELKEY, client->getNickname(), ERRSTRING_BADCHANNELKEY(channelName)));
		if (channel->isInviteOnly() && !channel->isInvited(client))
			return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_INVITEONLYCHAN, client->getNickname(), ERRSTRING_INVITEONLYCHAN(channelName)));
		if (channel->isFull())
			return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_CHANNELISFULL, client->getNickname(), ERRSTRING_CHANNELISFULL(channelName)));
		if (channel->isBanned(client))
			return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_BANNEDFROMCHAN, client->getNickname(), ERRSTRING_BANNEDFROMCHAN(channelName)));
	}
		//this part can be done in the channel Directly
	channel->addClient(cmd.client);
	cmd.client->joinChannel(channelName);

	std::string joinMsg = MessageMaker::MessageGenerator(client->getPrefix(), "JOIN", channelName);
	channel->broadcast(joinMsg, cmd.client);

	// RPL_TOPIC (332) ou "no topic" (331)
	if (!channel->getTopic().empty()) 
		cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_TOPIC, client->getNickname(), RPLSTRING_TOPIC(channelName, channel->getTopic())));
	else 
		cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_NOTOPIC, client->getNickname(), RPLSTRING_NOTOPIC));
}



//i could add a feedback struct to let the command manager know what happens with the command
// it could contain a bool that indicate if a message has been added
// it could indicate if the command has been executed
void CmdJoin::execute(const CommandData& cmd)
{
	// ERR_NEEDMOREPARAMS (461)
	if (cmd.args.empty())
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	if (cmd.args[0] == "0")	
		return _channelManager->removeClientFromAllChannels(cmd.client, "");
	std::string keysArg = (cmd.args.size() > 1) ? cmd.args[1] : "";
	std::map<std::string, std::string> channelList = CmdUtils::getChannelListFromData(cmd);

	for (std::map<std::string, std::string>::iterator it = channelList.begin(); it != channelList.end(); ++it)
	{
		const std::string& channelName = it->first;
		const std::string& key = it->second;
		tryJoinChannel(channelName, key, cmd.client, cmd);
	}
}

//TODO : channel isFull isBanned(client) isInviteOnly IsInvited(client) hasKey setKey(std::string) getTopic setTopic(std::string) 
