/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:07:31 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:12:31 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/CommandManager.hpp"
#include "manager/ServerManager.hpp"
#include "utils/CommandMaker.hpp"

CommandManager::CommandManager() {}

void printCommandData(const CommandData& cmd) {
	std::cout << "----- CommandData -----" << std::endl;
	std::cout << "Client pointer: " << cmd.client << std::endl;
	std::cout << "Prefix        : " << cmd.prefix << std::endl;
	std::cout << "Command       : " << cmd.cmd << std::endl;
	std::cout << "Arguments     :" << std::endl;
	for (size_t i = 0; i < cmd.args.size(); ++i) {
		std::cout << "  [" << i << "] " << cmd.args[i] << std::endl;
	}
	std::cout << "Hash          : " << cmd.hash << std::endl;
	std::cout << "------------------------" << std::endl;
}

CommandManager::CommandManager(ServerManager* serverManager)
: _serverManager(serverManager),
  _cmd_pass(),
  _cmd_nick(_serverManager->getClientManager(), _serverManager->getPasswordManager().getPassword()),
  _cmd_user(_serverManager->getPasswordManager().getPassword()),
  _cmd_join(_serverManager->getChannelManager()),
  _cmd_quit(_serverManager->getPollFDManager(), _serverManager->getChannelManager(), _serverManager->getClientManager()),
  _cmd_ping("ircserv"),
  _cmd_pong(),
  _cmd_privmsg(_serverManager->getChannelManager(), _serverManager->getClientManager()),
  _cmd_kick(_serverManager->getChannelManager(), _serverManager->getClientManager()),
  _cmd_invite(_serverManager->getChannelManager(), _serverManager->getClientManager()),
  _cmd_topic(_serverManager->getChannelManager(), _serverManager->getClientManager()),
  _cmd_mode(_serverManager->getChannelManager(), _serverManager->getClientManager())
{}

CommandManager::CommandManager(const CommandManager &other)
    : _serverManager(other._serverManager),
      _cmd_pass(),
      _cmd_nick(other._serverManager->getClientManager(),
                other._serverManager->getPasswordManager().getPassword()),
      _cmd_user(other._serverManager->getPasswordManager().getPassword()),
      _cmd_join(other._serverManager->getChannelManager()),
      _cmd_quit(other._serverManager->getPollFDManager(),
                other._serverManager->getChannelManager(),
                other._serverManager->getClientManager()),
      _cmd_ping("ircserv"),
      _cmd_pong(),
      _cmd_privmsg(other._serverManager->getChannelManager(),
                  other._serverManager->getClientManager()),
      _cmd_kick(other._serverManager->getChannelManager(),
                other._serverManager->getClientManager()),
      _cmd_invite(other._serverManager->getChannelManager(),
                  other._serverManager->getClientManager()),
      _cmd_topic(other._serverManager->getChannelManager(),
                 other._serverManager->getClientManager()),
      _cmd_mode(other._serverManager->getChannelManager(),
                other._serverManager->getClientManager())
{}
CommandManager::~CommandManager() {}

CommandManager &CommandManager::operator=(const CommandManager &other) {
	if (this != &other) {
		_cmds_client = other._cmds_client;
	}
	return *this;
}

void CommandManager::Update(std::vector<Client *> upd_clients) {
	LogManager::logServerTech("CommandManager Update");
	for (std::vector<Client *>::iterator it = upd_clients.begin(); it != upd_clients.end(); ++it) {
		Client &client = **it;
		std::vector<ClientMessage_t>* messages = client.getMsgReceived();
		for (std::vector<ClientMessage_t>::iterator mit = messages->begin(); mit != messages->end(); ++mit) {
			CommandData cmd = CommandMaker::msgToCommand(&client, &(*mit));
			LogManager::logServerTech(cmd.toString());
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
        case H_PART:    return;
        case H_PRIVMSG: return _cmd_privmsg.execute(cmd);
        case H_KICK:	return _cmd_kick.execute(cmd);
        case H_INVITE:  return _cmd_invite.execute(cmd);
        case H_TOPIC:   return _cmd_topic.execute(cmd);
        case H_MODE:    return _cmd_mode.execute(cmd);
        default:
            break;
    }
}

std::string CommandData::toString() const {
        std::ostringstream oss;

        oss << "CommandData { ";
        oss << "prefix=\"" << prefix << "\", ";
        oss << "cmd=\"" << cmd << "\", ";
        oss << "args=[";

        for (size_t i = 0; i < args.size(); ++i) {
            oss << "\"" << args[i] << "\"";
            if (i != args.size() - 1)
                oss << ", ";
        }
        oss << "], ";

        if (client)
            oss << "client=" << client->getNickname() << ", ";
        else
            oss << "client=NULL, ";

        oss << "hash=" << hash;
        oss << " }";

        return oss.str();
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

