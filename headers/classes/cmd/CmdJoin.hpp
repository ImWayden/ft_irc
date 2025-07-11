/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdJoin.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 00:14:31 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 19:21:17 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDJOIN_HPP
# define CMDJOIN_HPP

#include "Cmd.hpp"
#include "manager/ChannelManager.hpp"

#include <vector>
#include <map>


//ERR_NEEDMOREPARAMS
#define ERRCODE_NEEDMOREPARAMS 461
#define ERRSTRING_NEEDMOREPARAMS(command) command + " :Not enough parameters"
//ERR_NOSUCHCHANNEL
#define ERRCODE_NOSUCHCHANNEL 403
#define ERRSTRING_NOSUCHCHANNEL(channel) channel + " :No such channel"
/*	ERR_TOOMANYCHANNELS
**	Sent to a user when they have joined the maximum
**  number of allowed channels and they try to join
**	another channel.
*/
#define ERRCODE_TOOMANYCHANNELS 405
#define ERRSTRING_TOOMANYCHANNELS(channel) channel + " :You have joined too many channels"
//ERR_TOOMANYTARGETS
#define ERRCODE_TOOMANYTARGETS 407
#define ERRSTRING_TOOMANYTARGETS(target, abort_msg) ((target) + " :407 recipients. " + (abort_msg))
//ERR_CHANNELISFULL
#define ERRCODE_CHANNELISFULL 471
#define ERRSTRING_CHANNELISFULL(channel) channel + " :Cannot join channel (+l)"
//ERR_INVITEONLYCHAN
#define ERRCODE_INVITEONLYCHAN 473
#define ERRSTRING_INVITEONLYCHAN(channel) channel + " :Cannot join channel (+i)"
//ERR_BANNEDFROMCHAN
#define ERRCODE_BANNEDFROMCHAN 474 
#define ERRSTRING_BANNEDFROMCHAN(channel) channel + " :Cannot join channel (+b)"
//ERR_BADCHANNELKEY
#define ERRCODE_BADCHANNELKEY 475
#define ERRSTRING_BADCHANNELKEY(channel) channel + " :Cannot join channel (+k)"
//ERR_BADCHANMASK
#define ERRCODE_BADCHANMASK 476
#define ERRSTRING_BADCHANMASK(channel) channel + " :Bad Channel Mask"

class CmdJoin
{
public:
	CmdJoin();
	~CmdJoin();
	CmdJoin(const CmdJoin &other);
	CmdJoin &operator=(const CmdJoin &other);
	void execute(const CommandData &cmd);

private:
	bool isValidChannelName(std::string name);
	std::vector<std::string> split(const std::string& s, char delimiter);
	std::map<std::string, std::string> CmdJoin::getChannelListFromData(const CommandData& cmd);
	ChannelManager *_channelManager;
};



#endif