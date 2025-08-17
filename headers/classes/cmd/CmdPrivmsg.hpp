/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:14:40 by wayden            #+#    #+#             */
/*   Updated: 2025/08/17 13:28:00 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PRIVMSG_HPP
# define CMD_PRIVMSG_HPP

#include "utils/CmdUtils.hpp"
#include "manager/ChannelManager.hpp"
#include "manager/ClientManager.hpp"

#define ERCCODE_NOTEXTTOSEND "412"
#define ERRSTRING_NOTEXTTOSEND ":No text to send"

#define ERRCODE_NORECIPIENT "411"
#define ERRSTRING_NORECIPIENT(command) ":No recipient given (" + command + ")"

enum recipientType {
	USER,
	CHANNEL
};


struct target {
	Client *client;
	Channel *channel;
	std::string name;
	enum { NONE, NOSUCHNICK, NOSUCHCHANNEL, NORECIPIENT, TOOMANYTARGETS } error;
};


class CmdPrivmsg{
public:
	CmdPrivmsg();
	CmdPrivmsg(ChannelManager &channelManager, ClientManager &clientManager);
	CmdPrivmsg(CmdPrivmsg const & src);
	~CmdPrivmsg();
	CmdPrivmsg & operator=(CmdPrivmsg const & rhs);
	void execute(const CommandData &cmd);
private:
	void msgtargetParser(const std::string &msgtarget, std::vector<target> &targets);
	void resolveClientTarget(const std::string &input, target &target);
	
	ChannelManager *_channelmanager;
	ClientManager *_clientmanager;
};


#endif // CMD_PRIVMSG_HPP