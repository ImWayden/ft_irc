/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:54:15 by wayden            #+#    #+#             */
/*   Updated: 2025/07/15 20:56:52 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPrivmsg.hpp"


CmdPrivmsg::CmdPrivmsg() {}

CmdPrivmsg::CmdPrivmsg(ChannelManager *channelmanager, ClientManager *clientmanager) : _channelmanager(channelmanager), _clientmanager(clientmanager) {}

CmdPrivmsg::~CmdPrivmsg() {}

CmdPrivmsg::CmdPrivmsg(CmdPrivmsg const & src) : _channelmanager(src._channelmanager), _clientmanager(src._clientmanager){}

CmdPrivmsg & CmdPrivmsg::operator=(CmdPrivmsg const & rhs) {
	if (this != &rhs) {
		_channelmanager = rhs._channelmanager;
		_clientmanager = rhs._clientmanager;
	}
	return *this;
}

Client* CmdPrivmsg::resolveClientTarget(const std::string& input) {
	size_t excl = input.find('!');
	size_t percent = input.find('%');
	size_t at = input.find('@');

	if (excl != std::string::npos && at != std::string::npos && excl < at) {
		std::string nick = input.substr(0, excl);
		return _clientmanager->getClientByNickname(nick);
	}
	else if (percent != std::string::npos) {
		std::string user = input.substr(0, percent);
		std::string host = input.substr(percent + 1);
		return _clientmanager->getClientByUserAndHost(user, host);
	}
	else if (at != std::string::npos) {
		std::string user = input.substr(0, at);
		return _clientmanager->getClientByUsername(user);
	}
	else {
		return _clientmanager->getClientByNickname(input);
	}
}


//return true if the recipient is a channel false otherwise
// idk if i should use bool since we do not usually expect a true false statement to define a type
// but i have only 2 type and do not need more than a bool so idk
void CmdPrivmsg::msgtargetParser(const std::string &msgtarget, std::vector<target> &targets) {
	std::vector<std::string> names = CmdUtils::split(msgtarget, ',');
	Client *client;
	Channel *channel;
	recipientType type;
	
	for(int i = 0; i < names.size(); i++) {
		if(CmdUtils::isValidChannelName(names[i])) {
			channel = _channelmanager->getChannel(names[i]);
			client = NULL;
		}
		else if(names[i][0] != '#' && names[i][0] != '&' && names[i][0] != '+' && names[i][0] != '%')
		{
			client = resolveClientTarget(names[i]);
			channel = NULL;
		}
		else
			client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NORECIPIENT, ERRSTRING_NORECIPIENT(cmd.cmd)));
		targets.push_back({client, channel});
	}
}


void CmdPrivmsg::execute(const CommandData &cmd) {
	Client *client = cmd.client;
	if(cmd.args.size() < 1 || cmd.args[0].empty())
		return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NORECIPIENT, ERRSTRING_NORECIPIENT(cmd.cmd)));
	if(cmd.args.size() < 2 || cmd.args[1].empty())
		return client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERCCODE_NOTEXTTOSEND, ERRSTRING_NOTEXTTOSEND));
	std::vector<target> targets;
	msgtargetParser(cmd.args[0], targets);
	std::string message = cmd.args[1];
	for(int i = 0; i < targets.size(); i++) {
		if(targets[i].client != NULL)
			targets[i].client->addMessage_out(MessageMaker::MessageGenerator(cmd, true, 0, message));
		else if(targets[i].channel != NULL)
			targets[i].channel->broadcast(MessageMaker::MessageGenerator(cmd, true, 0, message), client);
		else
			client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NORECIPIENT, ERRSTRING_NORECIPIENT(cmd.cmd)));
	}
}

/*
      Command: PRIVMSG
   Parameters: <msgtarget> <text to be sent>

   PRIVMSG is used to send private messages between users, as well as to
   send messages to channels.  <msgtarget> is usually the nickname of
   the recipient of the message, or a channel name.

   The <msgtarget> parameter may also be a host mask (#<mask>) or server
   mask ($<mask>).  In both cases the server will only send the PRIVMSG
   to those who have a server or host matching the mask.  The mask MUST
   have at least 1 (one) "." in it and no wildcards following the last
   ".".  This requirement exists to prevent people sending messages to
   "#*" or "$*", which would broadcast to all users.  Wildcards are the
   '*' and '?'  characters.  This extension to the PRIVMSG command is
   only available to operators.

   Numeric Replies:

           ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY

   Examples:

   :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
                                   ; Message from Angel to Wiz.

   PRIVMSG Angel :yes I'm receiving it !
                                   ; Command to send a message to Angel.

   PRIVMSG jto@tolsun.oulu.fi :Hello !
                                   ; Command to send a message to a user
                                   on server tolsun.oulu.fi with
                                   username of "jto".

   PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
                                   ; Message to a user on server
                                   irc.stealth.net with username of
                                   "kalt", and connected from the host
                                   millennium.stealth.net.

   PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
                                   ; Message to a user on the local
                                   server with username of "kalt", and
                                   connected from the host
                                   millennium.stealth.net.

   PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
                                   ; Message to the user with nickname
                                   Wiz who is connected from the host
                                   tolsun.oulu.fi and has the username
                                   "jto".

   PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
                                   ; Message to everyone on a server
                                   which has a name matching *.fi.

   PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
                                   ; Message to all users who come from
                                   a host which has a name matching
                                   *.edu.


*/