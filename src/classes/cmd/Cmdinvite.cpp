/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmdinvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:28:09 by wayden            #+#    #+#             */
/*   Updated: 2025/08/18 08:59:22 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdInvite.hpp"

CmdInvite::CmdInvite() {}

CmdInvite::CmdInvite(ChannelManager &channelManager, ClientManager &clientManager) : _channelmanager(&channelManager), _clientmanager(&clientManager) {}
CmdInvite::~CmdInvite() {}

CmdInvite::CmdInvite(const CmdInvite &other) : _channelmanager(other._channelmanager), _clientmanager(other._clientmanager){}

CmdInvite &CmdInvite::operator=(const CmdInvite &other) {
	if (this != &other) {
		_channelmanager = other._channelmanager;
		_clientmanager = other._clientmanager;
	}
	return *this;
}

void CmdInvite::execute(const CommandData &cmd) {
	Client *sender = cmd.client;
	if(cmd.args.size() < 2)
		return sender->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, sender->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	std::string receiverNick = cmd.args[0];
	std::string channelName = cmd.args[1];
	Client *receiver = _clientmanager->getClientByNickname(receiverNick);
	Channel *channel = _channelmanager->getChannel(channelName);
	if(receiver == NULL)
		return sender->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NOSUCHNICK, sender->getNickname(), ERRSTRING_NOSUCHNICK(sender->getNickname())));
	std::set<std::string> senderChannels = sender->getChannels();
	if(channel != NULL && senderChannels.find(channelName) != sender->getChannels().end())
	{
		if(channel->isInviteOnly() && !channel->isOperator(sender))
			return sender->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_CHANOPRIVSNEEDED, ERRSTRING_CHANOPRIVSNEEDED(channelName)));
		if(channel->isBanned(receiver) || receiver->getChannels().find(channelName) != receiver->getChannels().end())
			return sender->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_USERONCHANNEL, sender->getNickname(), ERRSTRING_USERONCHANNEL(receiverNick, channelName)));
	}
	else if(channel != NULL)
		return sender->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NOTONCHANNEL, sender->getNickname(), ERRSTRING_NOTONCHANNEL(channelName)));
		
	sender->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_INVITING, sender->getNickname(), RPLSTRING_INVITING(receiverNick, channelName)));
	receiver->addMessage_out(MessageMaker::MessageGenerator(sender->getNickname(), "INVITE", receiverNick, channelName));
	channel->Invite(receiver);
}
