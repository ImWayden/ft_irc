/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:07:31 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:01:26 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/CommandManager.hpp"

CommandManager::CommandManager() 
{
	_cmd_pass = CmdPass();
	_cmd_nick = CmdNick(_serverManager->getClientManager());
	_cmd_user = CmdUser();
	_cmd_join = CmdJoin();
	_cmd_quit = CmdQuit();
	_cmd_ping = CmdPing();
	_cmd_pong = CmdPong();
	_cmd_part = CmdPart();
	_cmd_privmsg = CmdPrivmsg();
	_cmd_kick = CmdKick();
	_cmd_invite = CmdInvite();
	_cmd_topic = CmdTopic();
	_cmd_mode = CmdMode();
}

CommandManager::CommandManager(const CommandManager &other) : _commands_onhold(other._commands_onhold), _cmds_client(other._cmds_client) {}
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
        case H_PING:    cmd_ping(cmd); break;
        case H_PONG:    cmd_pong(cmd); break;
        case H_JOIN:    return _cmd_join.execute(cmd);
        case H_PART:    cmd_part(cmd); break;
        case H_PRIVMSG: cmd_privmsg(cmd); break;
        case H_KICK:    cmd_kick(cmd); break;
        case H_INVITE:  cmd_invite(cmd); break;
        case H_TOPIC:   cmd_topic(cmd); break;
        case H_MODE:    cmd_mode(cmd); break;
        default:
            // Commande inconnue ou non gérée
            break;
    }
}

//idk how to implement that should i add a global clock on the serv manager send a ping msg to client then wait create a commanddata struct 
//the command data struct would contain the time of the ping and the maximum time of the pong then every loop we check if we received a pong
void CommandManager::cmd_ping(const CommandData &cmd) 
{
	//answer with pong
	cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, true, 0, cmd.args[0]));
}

void CommandManager::cmd_pong(const CommandData &cmd) 
{
	//update the thing that send pings i guess
	cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, true, 0, cmd.args[0]));
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

