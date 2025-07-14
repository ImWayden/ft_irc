/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:35:47 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 17:43:26 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPass.hpp"


void CmdPass::execute(const CommandData &cmd)
{
	Client *client = cmd.client;
	
	if(client->isAuthenticated()) 
		return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_ALREADYREGISTRED, ERRSTRING_ALREADYREGISTRED));
	if(cmd.args.size() < 1)
		return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NEEDMOREPARAMS, ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	client->setPassword(cmd.args[0]);
}
