/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdQuit.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:10:59 by wayden            #+#    #+#             */
/*   Updated: 2025/07/28 03:09:58 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDQUIT_HPP
# define CMDQUIT_HPP

#include "utils/CmdUtils.hpp"
#include "manager/PollFDManager.hpp"
#include "manager/ChannelManager.hpp"
#include "manager/ClientManager.hpp"

class CmdQuit
{
public:
	CmdQuit();
	CmdQuit(PollFDManager &pollfdManager, ChannelManager &channelManager, ClientManager &clientManager);
	~CmdQuit();
	CmdQuit(const CmdQuit &other);
	CmdQuit &operator=(const CmdQuit &other);
	void execute(const CommandData &cmd);
private:
	PollFDManager* _pollfdManager;
	ChannelManager* _channelManager;
	ClientManager* _clientManager;
};

#endif