/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:22:47 by wayden            #+#    #+#             */
/*   Updated: 2025/07/13 12:41:37 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdKick.hpp"

CmdKick::CmdKick( void ) {}
CmdKick::CmdKick(ChannelManager *channelManager, ClientManager *clientManager) : _channelManager(channelManager), _clientManager(clientManager) {}
CmdKick::CmdKick( const CmdKick & src ) { *this = src; }
CmdKick::~CmdKick( void ) {}
CmdKick & CmdKick::operator=( const CmdKick & other ) 
{
	if (this != &other) {
		_channelManager = other._channelManager;
		_clientManager = other._clientManager;
	}
	return *this;
}


void CmdKick::tryKick(std::string channel, std::string user, const CommandData & cmd) 
{
	if(!CmdUtils::isValidChannelName(channel))
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_BADCHANMASK, ERRSTRING_BADCHANMASK(channel)));
	Channel* channelPtr = _channelManager->getChannel(channel);
	if(channelPtr == NULL)
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOSUCHCHANNEL, ERRSTRING_NOSUCHCHANNEL(channel)));
	if(!channelPtr->isOperator(cmd.client))
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_CHANOPRIVSNEEDED, ERRSTRING_CHANOPRIVSNEEDED(channel)));
	Client* userPtr = _clientManager->getClientByNickname(user);
	if(cmd.client->getChannels().find(channel) == cmd.client->getChannels().end())
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOTONCHANNEL, ERRSTRING_NOTONCHANNEL(channel)));
	if(channelPtr->isOperator(userPtr))
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_CHANOPRIVSNEEDED, ERRSTRING_CHANOPRIVSNEEDED(channel)));
	if(userPtr == NULL || userPtr->getChannels().find(channel) == userPtr->getChannels().end())
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_USERNOTONCHANNEL, ERRSTRING_USERNOTONCHANNEL(user, channel)));
	std::string message;
	if(cmd.args.size() > 2 && !cmd.args[2].empty())
		message = message = channelPtr->getName() + " " + userPtr->getNickname() + " (" + cmd.args[1] + ")";
	else
		message = channelPtr->getName() + " " + userPtr->getNickname() + " ( because he ugly as fuck )";
	channelPtr->broadcast(MessageMaker::MessageGenerator(cmd, true, 0, message, "KICK"), NULL);
	channelPtr->removeClient(userPtr);
}

void CmdKick::execute( const CommandData & cmd ) 
{
	Client *client = cmd.client;
	if(cmd.args.size() < 2) 
		return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	
	std::vector<std::string> channels = CmdUtils::split(cmd.args[0], ',');
	std::vector<std::string> users = CmdUtils::split(cmd.args[1], ',');
	bool uniqueChannel = channels.size() == 1;
	
	if(channels.size() != users.size() && !uniqueChannel) 
		return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	
	if(uniqueChannel)
	{
		if(CmdUtils::isValidChannelName(channels[0]) == false) 
			return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_BADCHANMASK, ERRSTRING_BADCHANMASK(channels[0])));
		for(std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it)
			tryKick(channels[0], *it, cmd);
	}
	else
		for(int i = 0; i < channels.size(); ++i)
			tryKick(channels[i], users[i], cmd);	
}



/*
	Kick command

      Command: KICK
   Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
               [<comment>]

   The KICK command can be used to request the forced removal of a user
   from a channel.  It causes the <user> to PART from the <channel> by
   force.  For the message to be syntactically correct, there MUST be
   either one channel parameter and multiple user parameter, or as many
   channel parameters as there are user parameters.  If a "comment" is
   given, this will be sent instead of the default message, the nickname
   of the user issuing the KICK.

   The server MUST NOT send KICK messages with multiple channels or
   users to clients.  This is necessarily to maintain backward
   compatibility with old client software.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
           ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

   Examples:

   KICK &Melbourne Matthew         ; Command to kick Matthew from
                                   &Melbourne

   KICK #Finnish John :Speaking English
                                   ; Command to kick John from #Finnish
                                   using "Speaking English" as the
                                   reason (comment).

   :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
                                   ; KICK message on channel #Finnish
                                   from WiZ to remove John from channel


       441    ERR_USERNOTINCHANNEL
              "<nick> <channel> :They aren't on that channel"

         - Returned by the server to indicate that the target
           user of the command is not on the given channel.
*/