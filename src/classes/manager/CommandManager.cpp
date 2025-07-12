/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:07:31 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 18:11:46 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/CommandManager.hpp"

CommandManager::CommandManager(ServerManager* serverManager)
{
	_cmd_pass = CmdPass();
	_cmd_nick = CmdNick(_serverManager->getClientManager());
	_cmd_user = CmdUser();
	_cmd_join = CmdJoin();
	_cmd_quit = CmdQuit(_serverManager->getPollFDManager(), _serverManager->getChannelManager(), _serverManager->getClientManager());
	_cmd_ping = CmdPing();
	_cmd_pong = CmdPong();
	_cmd_part = CmdPart();
	_cmd_privmsg = CmdPrivmsg();
	_cmd_kick = CmdKick();
	_cmd_invite = CmdInvite();
	_cmd_topic = CmdTopic();
	_cmd_mode = CmdMode();
}

CommandManager::CommandManager(const CommandManager &other) : _commands_onhold(other._commands_onhold), _cmds_client(other._cmds_client), _serverManager(other._serverManager) {}
CommandManager::~CommandManager() {}

CommandManager &CommandManager::operator=(const CommandManager &other) {
	if (this != &other) {
		_commands_onhold = other._commands_onhold;
		_cmds_client = other._cmds_client;
	}
	return *this;
}

void CommandManager::Update(std::vector<Client *> upd_clients) {
	for (std::vector<Client *>::iterator it = upd_clients.begin(); it != upd_clients.end(); ++it) {
		Client &client = **it;
		std::vector<ClientMessage_t>* messages = client.getMsgReceived();
		for (std::vector<ClientMessage_t>::iterator mit = messages->begin(); mit != messages->end(); ++mit) {
			CommandData cmd = CommandMaker::msgToCommand(&client, &(*mit));
			executeCommand(cmd);
			if (cmd.hash == H_QUIT)
				break;
		}
	}
}

void CommandManager::executeCommand(const CommandData &cmd) {
    switch(cmd.hash) {
        case H_PASS:   	return _cmd_pass.execute(cmd);
        case H_NICK:   	return _cmd_nick.execute(cmd);
        case H_USER:   	return _cmd_user.execute(cmd);
        case H_QUIT:    return _cmd_quit.execute(cmd);
        case H_PING:    return _cmd_ping.execute(cmd);
        case H_PONG:    return _cmd_pong.execute(cmd);
        case H_JOIN:    return _cmd_join.execute(cmd);
        case H_PART:    return _cmd_part.execute(cmd);
        case H_PRIVMSG: return _cmd_privmsg.execute(cmd);
        case H_KICK:	return _cmd_kick.execute(cmd);
        case H_INVITE:  return _cmd_invite.execute(cmd);
        case H_TOPIC:   return _cmd_topic.execute(cmd);
        case H_MODE:    return _cmd_mode.execute(cmd);
        default:
            // Commande inconnue ou non gérée
            break;
    }
}

/*
PASS
NICK
USER
QUIT
PING
PONG
JOIN
PART
PRIVMSG
KICK
INVITE
TOPIC
MODE (avec i, t, k, o, l)
*/

