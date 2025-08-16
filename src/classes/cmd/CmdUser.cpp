/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:51:58 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:00:44 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdUser.hpp"
#include <iostream>

CmdUser::CmdUser() {}

CmdUser::CmdUser(std::string serverpassword) : _serverpassword(serverpassword) {}

CmdUser::~CmdUser() {}

CmdUser::CmdUser(const CmdUser &other) : _serverpassword(other._serverpassword){}

CmdUser &CmdUser::operator=(const CmdUser &other) {
	_serverpassword = other._serverpassword;
	return *this;
}

bool CmdUser::checkusername(const std::string &username) {
	return(!(username.length() < NICK_MIN_LENGTH || username.find_first_of(R_ABNF_USER_NOUSECHAR) != std::string::npos));
}

void CmdUser::execute(const CommandData &cmd) 
{
	Client *client = cmd.client;
	bool isAuthenticated = client->isAuthenticated();
	if(cmd.args.size() < 1 || !checkusername(cmd.args[0]))
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	if(isAuthenticated == true)
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_ALREADYREGISTRED, client->getNickname(), ERRSTRING_ALREADYREGISTRED));
	client->setAuthStatus(USER_RECEIVED);
	client->setUsername(cmd.args[0]);
	if(!isAuthenticated && client->isAuthenticated())
	{
		if(client->getPassword() != _serverpassword)
		{
			client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_PASSWDMISMATCH, client->getNickname(), ERRSTRING_PASSWDMISMATCH));
			client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, "ERROR", ":disconnected from the server"));
			client->setQuitStatus(QUITTING);
			return;
		}
		else
		{
			client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_WELCOME, client->getNickname(), RPLSTRING_WELCOME(client->getPrefix())));
			client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_YOURHOST, client->getNickname(), "Your host is ircserv, running version 1.0"));
			client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_CREATED, client->getNickname(), RPLSTRING_CREATED));
			client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_MYINFO, client->getNickname(), "ircserv 1.0 +i +l"));
		}
	}
}
