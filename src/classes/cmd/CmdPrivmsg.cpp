/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 21:54:15 by wayden            #+#    #+#             */
/*   Updated: 2025/08/17 13:31:29 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPrivmsg.hpp"


CmdPrivmsg::CmdPrivmsg() {}

CmdPrivmsg::CmdPrivmsg(ChannelManager &channelmanager, ClientManager &clientmanager) : _channelmanager(&channelmanager), _clientmanager(&clientmanager) {}

CmdPrivmsg::~CmdPrivmsg() {}

CmdPrivmsg::CmdPrivmsg(CmdPrivmsg const & src) : _channelmanager(src._channelmanager), _clientmanager(src._clientmanager){}

CmdPrivmsg & CmdPrivmsg::operator=(CmdPrivmsg const & rhs) {
	if (this != &rhs) {
		_channelmanager = rhs._channelmanager;
		_clientmanager = rhs._clientmanager;
	}
	return *this;
}

void CmdPrivmsg::resolveClientTarget(const std::string& input, target &target) {
	size_t excl = input.find('!');
	size_t percent = input.find('%');
	size_t at = input.find('@');
	bool found_multiple = false;
	
	if (excl != std::string::npos && at != std::string::npos && excl < at) {
		std::string nick = input.substr(0, excl);
		target.client = _clientmanager->getClientByNickname(nick);
	}
	else if (percent != std::string::npos) {
		std::string user = input.substr(0, percent);
		std::string host = input.substr(percent + 1);
		target.client = _clientmanager->getClientByUserAndHost(user, host, found_multiple);
	}
	else if (at != std::string::npos) {
		std::string user = input.substr(0, at);
		target.client = _clientmanager->getClientByUsername(user, found_multiple);
	}
	else {
		target.client = _clientmanager->getClientByNickname(input);
	}
	if (found_multiple)
		target.error = target::TOOMANYTARGETS;
	else if (target.client == NULL)
		target.error = target::NOSUCHNICK;
}

void CmdPrivmsg::msgtargetParser(const std::string &msgtarget, std::vector<target> &targets) {
	std::vector<std::string> names = CmdUtils::split(msgtarget, ',');
	
	for(size_t i = 0; i < names.size(); i++) {
		target target = {NULL, NULL, names[i], target::NONE};
		if(CmdUtils::isValidChannelName(names[i])) {
			target.channel = _channelmanager->getChannel(names[i]);
			if(target.channel == NULL)
				target.error = target::NOSUCHCHANNEL;
		}
		else if(names[i][0] != '#' && names[i][0] != '&' && names[i][0] != '+' && names[i][0] != '%')
			resolveClientTarget(names[i], target);
		else
			target.error = target::NORECIPIENT;
		targets.push_back(target);
	}
}

//client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NOSUCHCHANNEL, client->getNickname(), ERRSTRING_NOSUCHCHANNEL(names[i])));
//client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NORECIPIENT, client->getNickname(), ERRSTRING_NORECIPIENT(cmd.cmd)));
void CmdPrivmsg::execute(const CommandData &cmd) {
	Client *client = cmd.client;
	if(cmd.args.size() < 1 || cmd.args[0].empty())
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NORECIPIENT, client->getNickname(), ERRSTRING_NORECIPIENT(cmd.cmd)));
	if(cmd.args.size() < 2 || cmd.args[1].empty())
		return client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERCCODE_NOTEXTTOSEND, client->getNickname(), ERRSTRING_NOTEXTTOSEND));
	std::vector<target> targets;
	msgtargetParser(cmd.args[0], targets);
	std::string message = cmd.args[1];
	for(size_t i = 0; i < targets.size(); i++) {
		switch(targets[i].error)
		{
			case target::NONE:
				if(targets[i].client != NULL)
					targets[i].client->addMessage_out(MessageMaker::MessageGenerator(client->getPrefix(), "PRIVMSG", targets[i].name, " :" + message));
				else if(targets[i].channel != NULL)
					targets[i].channel->broadcast(MessageMaker::MessageGenerator(client->getPrefix(), "PRIVMSG", targets[i].name, " :" + message), client);
				break;
			case target::NORECIPIENT:
				client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NORECIPIENT, client->getNickname(), ERRSTRING_NORECIPIENT(cmd.cmd)));
				break;
			case target::NOSUCHNICK:
				client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NOSUCHNICK, client->getNickname(), ERRSTRING_NOSUCHNICK(targets[i].name)));
				break;
			case target::NOSUCHCHANNEL:
				client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NOSUCHCHANNEL, client->getNickname(), ERRSTRING_NOSUCHCHANNEL(targets[i].name)));
				break;
			case target::TOOMANYTARGETS:
				client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_TOOMANYTARGETS, client->getNickname(), ERRSTRING_TOOMANYTARGETS(targets[i].name, "too many targets")));
				break;
		}	
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

	// Server to server messages (not required) 
   PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
                                   ; Message to everyone on a server
                                   which has a name matching *.fi.

   PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
                                   ; Message to all users who come from
                                   a host which has a name matching
                                   *.edu.


*/