/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:31:17 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 18:45:50 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdTopic.hpp"

CmdTopic::CmdTopic() {}

CmdTopic::CmdTopic(ChannelManager *channelmanager, ClientManager *clientmanager) : _channelmanager(channelmanager), _clientmanager(clientmanager) {}

CmdTopic::~CmdTopic() {}

CmdTopic::CmdTopic(const CmdTopic &other) : _channelmanager(other._channelmanager), _clientmanager(other._clientmanager){}

CmdTopic &CmdTopic::operator=(const CmdTopic &other) {
	if (this != &other) {
		_channelmanager = other._channelmanager;
		_clientmanager = other._clientmanager;
	}
	return *this;
}

void CmdTopic::execute(const CommandData &cmd) 
{
	if(cmd.args.size() < 1)
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	Channel *channel = _channelmanager->getChannel(cmd.args[0]);
	if(!channel)
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOSUCHCHANNEL, ERRSTRING_NOSUCHCHANNEL(cmd.args[0])));
	std::string topic = channel->getTopic();
	std::string channelname = channel->getName();
	if(cmd.args.size() == 1)
	{
		if(!topic.empty())
			return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, true, RPLCODE_TOPIC, RPLSTRING_TOPIC(channelname, topic)));
		else
			return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, true, RPLCODE_NOTOPIC, RPLSTRING_NOTOPIC(channelname)));
	}
	else
	{
		if(!channel->isOperator(cmd.client) && channel->isTopicProtected())
			return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_CHANOPRIVSNEEDED, ERRSTRING_CHANOPRIVSNEEDED(channelname)));
		channel->setTopic(cmd.args[1]);
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, true, RPLCODE_TOPIC, RPLSTRING_TOPIC(channelname, topic)));
	}
}

/*
      Command: TOPIC
   Parameters: <channel> [ <topic> ]

   The TOPIC command is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if this action is allowed for the user
   requesting it.  If the <topic> parameter is an empty string, the
   topic for that channel will be removed.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

   Examples:

   :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
                                   topic.

   TOPIC #test :another topic      ; Command to set the topic on #test
                                   to "another topic".

   TOPIC #test :                   ; Command to clear the topic on
                                   #test.

   TOPIC #test                     ; Command to check the topic for
                                   #test.
*/