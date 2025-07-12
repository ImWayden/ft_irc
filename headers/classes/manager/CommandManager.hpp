/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 18:04:58 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 01:35:21 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include "struct/struct.hpp"
#include <vector>

#include "ressources/r_numbers.hpp"
#include "PasswordManager.hpp"
#include "PortManager.hpp"
#include "PollFDManager.hpp"
#include "ListenerManager.hpp" 
#include "ClientManager.hpp"
#include "ServerManager.hpp"
#include "utils/CommandMaker.hpp"

#include "cmd/CmdPass.hpp"
#include "cmd/CmdNick.hpp"
#include "cmd/CmdUser.hpp"
#include "cmd/CmdJoin.hpp"
#include "cmd/CmdQuit.hpp"

#include "cmd/CmdPing.hpp"
#include "cmd/CmdPong.hpp"
#include "cmd/CmdPart.hpp"
#include "cmd/CmdPrivmsg.hpp"
#include "cmd/CmdKick.hpp"
#include "cmd/CmdInvite.hpp"
#include "cmd/CmdTopic.hpp"
#include "cmd/CmdMode.hpp"


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


// i don't like the idea of having to give it that much thing it i should probably merge it with the servermanager
// or i could have a struct to transfert instructions from class to class 
class CommandManager {
public:
	CommandManager();
	CommandManager(ServerManager* serverManager);
	CommandManager(const CommandManager &other);
	~CommandManager();
	CommandManager &operator=(const CommandManager &other);
	void Update(std::vector<Client *> upd_clients);
private:
	std::vector<int> _clientsfd_pullout_upd;
	std::vector<CommandData> _cmds_client; // List of commands received from clients
	std::vector<CommandData> _commands_onhold; // List of commands to be executed
	
	ServerManager* _serverManager;
	/* ---    cmd part --- */
	void executeCommand(const CommandData &cmd);
	CmdPass _cmd_pass;
	CmdNick _cmd_nick;
	CmdUser _cmd_user;
	CmdJoin _cmd_join;
	CmdQuit _cmd_quit;
	CmdPing _cmd_ping; //need to answer pong
	CmdPong _cmd_pong; //need to update pingstatus
	CmdPart _cmd_part;
	CmdPrivmsg _cmd_privmsg;
	CmdKick _cmd_kick;
	CmdInvite _cmd_invite;
	CmdTopic _cmd_topic;
	CmdMode _cmd_mode;
};



#endif // COMMANDMANAGER_HPP