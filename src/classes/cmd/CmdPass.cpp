/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:35:47 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 19:19:43 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPass.hpp"


void CmdPass::execute(const CommandData &cmd)
{
	Client *client = cmd.client;
	
	if(client->isAuthenticated()) 
	{
		//ERR_ALREADYREGISTRED
		return;
	}
	if(cmd.args.size() < 1)
	{
		//ERR_NEEDMOREPARAMS
		return;
	}
	client->setAuthStatus(PASSWORD_RECEIVED);
	client->setPassword(cmd.args[0]);
}


