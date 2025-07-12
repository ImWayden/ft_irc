/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdKick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:20:03 by wayden            #+#    #+#             */
/*   Updated: 2025/07/13 00:09:24 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDKICK_HPP
# define CMDKICK_HPP

#include "utils/CmdUtils.hpp"
#include "manager/ChannelManager.hpp"
#include "manager/ClientManager.hpp"


#define ERRCODE_USERNOTONCHANNEL 441
#define ERRSTRING_USERNOTONCHANNEL(user, channel) user + " " + channel + " :They aren't on that channel"

class CmdKick
{
public:
	CmdKick( void );
	CmdKick(ChannelManager *channelManager, ClientManager *clientManager );
	CmdKick( const CmdKick & src );
	~CmdKick( void );
	CmdKick & operator=( const CmdKick & rhs );
	void execute( const CommandData & cmd );
private:
	void tryKick(std::string channel, std::string user, const CommandData & cmd);
	
	ChannelManager *_channelManager;
	ClientManager *_clientManager;
};
#endif
