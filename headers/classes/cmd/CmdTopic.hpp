/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:29:16 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 18:44:41 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDTOPIC_HPP
# define CMDTOPIC_HPP

#include "utils/CmdUtils.hpp"
#include "manager/ChannelManager.hpp"
#include "manager/ClientManager.hpp"

class CmdTopic {
public:
	CmdTopic();
	CmdTopic(ChannelManager *channelmanager, ClientManager *clientmanager);
	~CmdTopic();
	CmdTopic(CmdTopic const & src);
	CmdTopic & operator=(CmdTopic const & rhs);
	void execute(const CommandData &cmd);
private:
	ChannelManager *_channelmanager;
	ClientManager *_clientmanager;
};

#endif