/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:35:47 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 14:35:52 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPass.hpp"
#include "data/Client.hpp"
//ajouter les differents constructeurs et destructeurs

CmdPass::CmdPass() {}

CmdPass::~CmdPass() {}

CmdPass::CmdPass(const CmdPass &other) { (void)other; }

CmdPass &CmdPass::operator=(const CmdPass &other) { (void)other; return *this; }

void CmdPass::execute(const CommandData &cmd)
{
	Client *client = cmd.client;
	
	if(client->isAuthenticated()) 
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_ALREADYREGISTRED, client->getNickname(), ERRSTRING_ALREADYREGISTRED));
	if(cmd.args.size() < 1)
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	client->setPassword(cmd.args[0]);
}
