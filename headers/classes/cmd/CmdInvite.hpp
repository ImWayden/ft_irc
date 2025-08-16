/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdInvite.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:23:53 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 10:02:37 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDINVITE_HPP
# define CMDINVITE_HPP

#include "utils/CmdUtils.hpp"
#include "manager/ClientManager.hpp"
#include "manager/ChannelManager.hpp"



#define ERRCODE_NOSUCHNICK "401"
#define ERRSTRING_NOSUCHNICK(nick) nick + " :No such nick/channel"
#define ERRCODE_USERONCHANNEL "443"
#define ERRSTRING_USERONCHANNEL(nick, channel) nick + " " + channel + " :is already on channel"
#define RPL_INVITING "341"

class CmdInvite{
public:
	CmdInvite();
	CmdInvite(ChannelManager &channelManager, ClientManager &clientManager);
	CmdInvite(const CmdInvite &other);
	~CmdInvite();
	CmdInvite &operator=(const CmdInvite &other);
	void execute(const CommandData &cmd);
private:
	ChannelManager* _channelmanager;
	ClientManager* _clientmanager;
};


#endif

/*
	401    ERR_NOSUCHNICK
              "<nickname> :No such nick/channel"

          - Used to indicate the nickname parameter supplied to a
            command is currently unused.
	442    ERR_NOTONCHANNEL
              "<channel> :You're not on that channel"

         - Returned by the server whenever a client tries to
           perform a channel affecting command for which the
           client isn't a member.

	443    ERR_USERONCHANNEL
              "<user> <channel> :is already on channel"

         - Returned when a client tries to invite a user to a
           channel they are already on.

	482    ERR_CHANOPRIVSNEEDED
              "<channel> :You're not channel operator"

         - Any command requiring 'chanop' privileges (such as
           MODE messages) MUST return this error if the client
           making the attempt is not a chanop on the specified
           channel.
	341    RPL_INVITING
              "<channel> <nick>"

         - Returned by the server to indicate that the
           attempted INVITE message was successful and is
           being passed onto the end client.
	301    RPL_AWAY
              "<nick> :<away message>"
		   
    Command: INVITE
   Parameters: <nickname> <channel>

   The INVITE command is used to invite a user to a channel.  The
   parameter <nickname> is the nickname of the person to be invited to
   the target channel <channel>.  There is no requirement that the
   channel the target user is being invited to must exist or be a valid
   channel.  However, if the channel exists, only members of the channel
   are allowed to invite other users.  When the channel has invite-only
   flag set, only channel operators may issue INVITE command.
   Only the user inviting and the user being invited will receive
   notification of the invitation.  Other channel members are not
   notified.  (This is unlike the MODE changes, and is occasionally the
   source of trouble for users.)

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
           ERR_NOTONCHANNEL                ERR_USERONCHANNEL
           ERR_CHANOPRIVSNEEDED
           RPL_INVITING                    RPL_AWAY

   Examples:

   :Angel!wings@irc.org INVITE Wiz #Dust

                                   ; Message to WiZ when he has been
                                   invited by user Angel to channel
                                   #Dust

   INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
                                   #Twilight_zone

*/
