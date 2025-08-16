/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:51:02 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:59:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdQuit.hpp"

CmdQuit::CmdQuit() {}
CmdQuit::CmdQuit(PollFDManager &pollfdManager, ChannelManager &channelManager, ClientManager &clientManager) : 
	_pollfdManager(&pollfdManager), _channelManager(&channelManager), _clientManager(&clientManager) {}

CmdQuit::~CmdQuit() {}

CmdQuit::CmdQuit(const CmdQuit &other) : _pollfdManager(other._pollfdManager), _channelManager(other._channelManager), _clientManager(other._clientManager) {}

CmdQuit &CmdQuit::operator=(const CmdQuit &other) {
	if (this != &other) {
		_pollfdManager = other._pollfdManager;
		_channelManager = other._channelManager;
		_clientManager = other._clientManager;
	}
	return *this;
}

void CmdQuit::execute(const CommandData &cmd) 
{
	std::set<std::string> channels = cmd.client->getChannels();
	std::string message;
	if(cmd.args.size() > 0)
		message = cmd.args[0];
	else
		message = "";
	_channelManager->removeClientFromAllChannels(cmd.client, message);
	cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, "ERROR", ":disconnected from the server")); //replace with ERROR msg
	cmd.client->setQuitStatus(QUITTING);
}

// TODO : need to rethink about channels in client should them be stored per name or through pointers to channels
// if stored like a list of pointer the porblem is that if a channel is deleted the pointer will be invalid
// so it let more room for dev errors 

// there is a big probleme with how this is managed i can't send a message to the client if i delete it before sending
// but sending requires me to wait for POLLOUT so waiting for the next server loop, technically i could add a command or bool 
// to force the client manager to send the message before deleting the client and then the same way the listener manager adds clients, a list of to remove clients
// i don't like the idea of waiting for the next loop and not being able to remove the client directly when the command is processed but whatever
// i could also just choose to not send anything because i don't a shit the fd will be closed and the client should not break if the server doesn't give an error back on disconnect
