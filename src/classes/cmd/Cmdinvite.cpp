/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmdinvite.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:28:09 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 21:46:20 by wayden           ###   ########.fr       */
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
		return sender->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	std::string receiverNick = cmd.args[0];
	std::string channelName = cmd.args[1];
	Client *receiver = _clientmanager->getClientByNickname(receiverNick);
	Channel *channel = _channelmanager->getChannel(channelName);
	if(receiver == NULL)
		return sender->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOSUCHNICK, ERRSTRING_NOSUCHNICK(channelName)));
	std::set<std::string> senderChannels = sender->getChannels();
	if(channel != NULL && senderChannels.find(channelName) != sender->getChannels().end())
	{
		if(channel->isInviteOnly() && !channel->isOperator(sender))
			return sender->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_CHANOPRIVSNEEDED, ERRSTRING_CHANOPRIVSNEEDED(channelName)));
		if(channel->isBanned(receiver) || receiver->getChannels().find(channelName) != receiver->getChannels().end())
			return sender->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_USERONCHANNEL, ERRSTRING_USERONCHANNEL(receiverNick, channelName)));
	}
	else if(channel != NULL)
		return sender->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOTONCHANNEL, ERRSTRING_NOTONCHANNEL(channelName)));
		
	sender->addMessage_out(MessageMaker::MessageGenerator(cmd, false, RPL_INVITING, channelName + " " + receiverNick));
	receiver->addMessage_out(MessageMaker::MessageGenerator(cmd, false, RPL_INVITING, channelName + " " + sender->getNickname()));
	channel->Invite(receiver);
	//not managing RPL_AWAY cause idgaf
}
