/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 00:14:31 by wayden            #+#    #+#             */
/*   Updated: 2025/08/17 17:25:21 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDJOIN_HPP
# define CMDJOIN_HPP

#include "utils/CmdUtils.hpp"
#include "manager/ChannelManager.hpp"

#include <vector>
#include <map>


//ERR_NEEDMOREPARAMS

//ERR_NOSUCHCHANNEL

/*	ERR_TOOMANYCHANNELS
**	Sent to a user when they have joined the maximum
**  number of allowed channels and they try to join
**	another channel.
*/
#define ERRCODE_TOOMANYCHANNELS "405"
#define ERRSTRING_TOOMANYCHANNELS(channel) channel + " :You have joined too many channels"

#define ERRCODE_CHANNELISFULL "471"
#define ERRSTRING_CHANNELISFULL(channel) channel + " :Cannot join channel (+l)"

#define ERRCODE_INVITEONLYCHAN "473"
#define ERRSTRING_INVITEONLYCHAN(channel) channel + " :Cannot join channel (+i)"

#define ERRCODE_BANNEDFROMCHAN "474" 
#define ERRSTRING_BANNEDFROMCHAN(channel) channel + " :Cannot join channel (+b)"

#define ERRCODE_BADCHANNELKEY "475"
#define ERRSTRING_BADCHANNELKEY(channel) channel + " :Cannot join channel (+k)"


class CmdJoin
{
public:
	CmdJoin();
	CmdJoin(ChannelManager& channelManager);
	CmdJoin(const CmdJoin &other);
	~CmdJoin();
	CmdJoin &operator=(const CmdJoin &other);
	void execute(const CommandData &cmd);

private:
	void tryJoinChannel(const std::string& channelName, const std::string& key, Client* client, const CommandData& cmd);
	std::map<std::string, std::string> getChannelListFromData(const CommandData& cmd);
	ChannelManager *_channelManager;
};



#endif