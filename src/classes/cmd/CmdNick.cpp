/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:44:07 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:58:46 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/cmd/CmdNick.hpp"
#include <iostream>

CmdNick::CmdNick() {}

CmdNick::CmdNick(ClientManager& clientManager, std::string serverpassword) : _clientManager(&clientManager), _serverpassword(serverpassword) {}

CmdNick::~CmdNick() {}

CmdNick::CmdNick(const CmdNick &other) : _clientManager(other._clientManager) {}

CmdNick& CmdNick::operator=(const CmdNick &other) {
    if (this != &other) {
        _clientManager = other._clientManager;
        _serverpassword = other._serverpassword;
    }
    return *this;
}


bool CmdNick::checknickname(const std::string &nickname) {
	// Check if the nickname contains invalid characters
	//std::string valid_chars = R_ABNF_ALPHA + (std::string)R_ABNF_DIGIT + R_ABNF_SPECIAL + '-';
	if(nickname.length() > NICK_MAX_LENGTH || nickname.length() < NICK_MIN_LENGTH || nickname[0] == '-' || nickname.find_first_not_of(R_ABNF_NICKNAME) != std::string::npos)
		return false;
	return true;
}

void CmdNick::execute(const CommandData &cmd) 
{
	Client *client = cmd.client;
	bool isAuthenticated = client->isAuthenticated();
	if(cmd.args.size() < 1)
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NONICKNAMEGIVEN, client->getNickname(), ERRSTRING_NONICKNAMEGIVEN));
	if(!checknickname(cmd.args[0]))
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME,  ERRCODE_ERRONEUSNICKNAME, cmd.args[0], ERRSTRING_ERRONEUSNICKNAME(cmd.args[0])));
	std::set<std::string> nicknames = _clientManager->getNicknames(); //using a set for faster search
	if(nicknames.find(cmd.args[0]) != nicknames.end())
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NICKNAMEINUSE, cmd.args[0], ERRSTRING_NICKNAMEINUSE(cmd.args[0])));
	client->setAuthStatus(NICK_RECEIVED);
	client->setNickname(cmd.args[0]);
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
