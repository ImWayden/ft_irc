/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:44:07 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 18:14:31 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "classes/cmd/CmdNick.hpp"

CmdNick::CmdNick() {}

CmdNick::CmdNick(ClientManager& clientManager) : _clientManager(&clientManager) {}

CmdNick::~CmdNick() {}

CmdNick::CmdNick(const CmdNick &other) : _clientManager(other._clientManager) {}

CmdNick &CmdNick::operator=(const CmdNick &other) {
	if (this != &other) {
		_clientManager = other._clientManager;
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
	{
		//ERR_NONICKNAMEGIVEN
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NONICKNAMEGIVEN, ":No nickname given"));
		return;
	}
	if(!checknickname(cmd.args[0]))
	{
		//ERR_ERRONEUSNICKNAME
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false,  ERRCODE_ERRONEUSNICKNAME, cmd.args[0] + " :Erroneus nickname"));
		return;
	}
	std::set<std::string> nicknames = _clientManager->getNicknames(); //using a set for faster search
	if(nicknames.find(cmd.args[0]) != nicknames.end())
	{
		//ERR_NICKNAMEINUSE
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NICKNAMEINUSE, cmd.args[0] + " :Nickname is already in use"));
		return;
	}
	client->setAuthStatus(NICK_RECEIVED);
	client->setNickname(cmd.args[0]);

	if(!isAuthenticated && client->isAuthenticated())
	{
		//methode to check if password is correct and to prepare a commanddata to send to the client
		
	}
	//ERR_RESTRICTED no need to implement since we don't have to implement mode for users
	//ERR_UNAVAILRESOURCE is it really necessary ? i don't think i will implement the lock on username after changing or deconnecting
	//ERR_NICKCOLLISION no need to implement since we only have one server
}
