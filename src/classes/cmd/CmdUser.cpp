/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:51:58 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 19:47:48 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




#include "cmd/CmdUser.hpp"

bool CmdUser::checkusername(const std::string &username) {
	return(!(username.length() < NICK_MIN_LENGTH || username.find_first_of(R_ABNF_USER_NOUSECHAR) != std::string::npos));
}

void CmdUser::execute(const CommandData &cmd) 
{
	Client *client = cmd.client;
	bool isAuthenticated = client->isAuthenticated();
	if(cmd.args.size() < 1 || !checkusername(cmd.args[0]))
	{
		//ERR_NEEDMOREPARAMS
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, 461, cmd.cmd + " :Not enough parameters")); //add defines for errors or enum perhaps
		return;
	}
	if(isAuthenticated == true)
	{
		//ERR_ALREADYREGISTRED
		client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, 462, ":Unauthorized command (already registered)"));
		return;
	}
	client->setAuthStatus(USER_RECEIVED);
	client->setUsername(cmd.args[0]);
	if(isAuthenticated == false && client->isAuthenticated())
	{
		//methode to check if password is correct and to prepare a commanddata to send welcome to the client
	}
}
