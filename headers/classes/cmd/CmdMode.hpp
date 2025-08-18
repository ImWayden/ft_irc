/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:32:40 by wayden            #+#    #+#             */
/*   Updated: 2025/08/18 12:41:05 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/CmdUtils.hpp"
#include "manager/ChannelManager.hpp"
#include "manager/ClientManager.hpp"
#include <sstream>

#ifndef CMDMODE_HPP
# define CMDMODE_HPP

#define ERRCODE_KEYSET "467"
#define ERRSTRING_KEYSET(channel) channel + " :Channel key already set"

#define ERRCODE_NOCHANMODES "477"
#define ERRSTRING_NOCHANMODES(channel) channel + " :Channel doesn't support modes"

#define ERRCODE_UNKNOWNMODE "472"
#define ERRSTRING_UNKNOWNMODE(mode, channel) mode + " :is unknown mode char to me for " + channel
class CmdMode
{
public:
	CmdMode();
	CmdMode(ChannelManager &channelManager, ClientManager &clientManager);
	~CmdMode();
	CmdMode(CmdMode const & src);
	CmdMode & operator=(CmdMode const & rhs);
	void execute(const CommandData & cmd);
private:
	void Mode(const CommandData &cmd, Channel *channel);
	
	void handle_k(bool isPlus, Channel *channel, const std::string &channelName, const std::string* parameter, size_t &parametersUsed, const CommandData &cmd);
	void handle_l(bool isPlus, Channel *channel, const std::string* parameter, size_t &parametersUsed, const CommandData &cmd);
	void handle_o(bool isPlus, Channel *channel, const std::string &channelName, const std::string* parameter, size_t &parametersUsed, const CommandData &cmd);

	ChannelManager *_channelmanager;
	ClientManager *_clientmanager;
};

#endif