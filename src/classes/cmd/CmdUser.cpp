/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:51:58 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 20:26:08 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdUser.hpp"

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
	{
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
		return;
	}
	if(isAuthenticated == true)
	{
		//ERR_ALREADYREGISTRED
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_ALREADYREGISTRED, ERRSTRING_ALREADYREGISTRED));
		return;
	}
	client->setAuthStatus(USER_RECEIVED);
	client->setUsername(cmd.args[0]);
	if(isAuthenticated == false && client->isAuthenticated())
	{
		if(client->getPassword() != _serverpassword)
		{
			client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_PASSWDMISMATCH, ERRSTRING_PASSWDMISMATCH));
			client->setQuitStatus(QUITTING);
			return;
		}
	}
}
