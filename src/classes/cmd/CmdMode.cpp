/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:33:55 by wayden            #+#    #+#             */
/*   Updated: 2025/08/18 13:01:18 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdMode.hpp"

CmdMode::CmdMode() {}

CmdMode::CmdMode(ChannelManager &channelManager, ClientManager &clientManager) : _channelmanager(&channelManager), _clientmanager(&clientManager) {}

CmdMode::~CmdMode() {}

CmdMode &CmdMode::operator=(const CmdMode &rhs) {
	if (this != &rhs) {
		_channelmanager = rhs._channelmanager;
		_clientmanager = rhs._clientmanager;
	}
	return *this;
}

CmdMode::CmdMode(const CmdMode &other) : _channelmanager(other._channelmanager), _clientmanager(other._clientmanager) {}


void CmdMode::handle_k(bool isPlus, Channel *channel, const std::string &channelName, const std::string* parameter, size_t &parametersUsed, const CommandData &cmd) 
{
	if(!isPlus)
		channel->setKey("");
	else if(parameter == NULL)
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	else if(channel->getKey().empty())
	{
		channel->setKey(*parameter);
		parametersUsed++;
	}
	else
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_KEYSET, ERRSTRING_KEYSET(channelName)));
	channel->broadcast(MessageMaker::MessageGenerator(cmd.client->getPrefix(), "MODE", channelName, (isPlus ? std::string("+k ") + *parameter : "-k")), NULL);
}

void CmdMode::handle_l(bool isPlus, Channel *channel, const std::string* parameter, size_t &parametersUsed, const CommandData &cmd) 
{
	int limit;
	if(!isPlus)
		channel->setLimit(MAX_CLIENTS);
	else if(parameter == NULL)
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	else
	{
		std::istringstream iss(*parameter);
		if (!(iss >> limit))
			return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
		channel->setLimit(limit);
		parametersUsed++;
	}
	channel->broadcast(MessageMaker::MessageGenerator(cmd.client->getPrefix(), "MODE", channel->getName(), (isPlus ? std::string("+l ") + *parameter : "-l")), NULL);
}

void CmdMode::handle_o(bool isPlus, Channel *channel, const std::string &channelName, const std::string* parameter, size_t &parametersUsed, const CommandData &cmd) 
{
	Client* target = _clientmanager->getClientByNickname(*parameter);
	
	if(parameter == NULL)
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	else if(target == NULL || target->getChannels().find(channelName) == target->getChannels().end())
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_USERNOTONCHANNEL, cmd.client->getNickname(), ERRSTRING_USERNOTONCHANNEL(*parameter, channelName)));
	else if(isPlus)
		channel->addOperator(target);
	else if (!isPlus && channel->isOperator(target)) 
		channel->removeOperator(target);
	channel->broadcast(MessageMaker::MessageGenerator(cmd.client->getPrefix(), "MODE", channelName, std::string(isPlus ? "+o " : "-o ") + *parameter), NULL);
	parametersUsed++;
}



void CmdMode::Mode(const CommandData &cmd, Channel *channel) {

	std::string channelName = cmd.args[0];
	std::string mode = cmd.args[1];
	std::vector<std::string> parameters(cmd.args.begin() + 2, cmd.args.end());
	std::string* actualparameter = &parameters[0];
	bool isPlus = false;
	size_t parametersUsed = 0;
	if(mode[0] == '+')
		isPlus = true;
	for(size_t i = 1; i < mode.length(); i++){
		switch(mode[i]){
			case 'i': channel->setInviteOnly(isPlus); channel->broadcast(MessageMaker::MessageGenerator(cmd.client->getPrefix(), "MODE", channelName, (isPlus ? "+i" : "-i")), NULL); break;
			case 'k': handle_k(isPlus, channel, channelName, actualparameter, parametersUsed, cmd); break;
			case 'l': handle_l(isPlus, channel, actualparameter, parametersUsed, cmd); break;
			case 'o': handle_o(isPlus, channel, channelName, actualparameter, parametersUsed, cmd); break;
			case 't': channel->setTopicProtected(isPlus); channel->broadcast(MessageMaker::MessageGenerator(cmd.client->getPrefix(), "MODE", channelName, (isPlus ? "+t" : "-t")), NULL); break;
			default: cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_UNKNOWNMODE, ERRSTRING_UNKNOWNMODE(mode[i], channelName)));break;
		};
		if(parametersUsed < parameters.size())
			actualparameter = &parameters[parametersUsed];
		else
			actualparameter = NULL;
	}
}

void CmdMode::execute(const CommandData &cmd) 
{
	if (cmd.args.empty())
        return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	std::string target = cmd.args[0];
	 if (target.empty() || (target[0] != '#' && target[0] != '&' && target[0] != '+' && target[0] != '!'))
        return; // silencieux
	if (cmd.args.size() >= 2 && (cmd.args[1][0] != '+' && cmd.args[1][0] != '-'))
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NEEDMOREPARAMS, cmd.client->getNickname(), ERRSTRING_NEEDMOREPARAMS(cmd.cmd)));
	Channel *channel = _channelmanager->getChannel(target);
	if (channel == NULL || cmd.client->getChannels().find(target) == cmd.client->getChannels().end())
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_NOTONCHANNEL, cmd.client->getNickname(), ERRSTRING_NOTONCHANNEL(target)));
	if (cmd.args.size() == 1) {
        std::string modes = channel->getModesString();
        std::string params = channel->getModeParams();
        cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, RPLCODE_CHANNELMODEIS, target, RPLSTRING_CHANNELMODEIS(target, modes + params)));
        return;
    }
	if(!channel->isOperator(cmd.client))
		return cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, ERRCODE_CHANOPRIVSNEEDED, ERRSTRING_CHANOPRIVSNEEDED(target)));
	Mode(cmd, channel);
}



/*

MODE - Changer le mode du channel :
— i : Définir/supprimer le canal sur invitation uniquement
— t : Définir/supprimer les restrictions de la commande TOPIC pour les opérateurs de canaux
— k : Définir/supprimer la clé du canal (mot de passe)
— o : Donner/retirer le privilège de l’opérateur de canal
— l : Définir/supprimer la limite d’utilisateurs pour le canal



467    ERR_KEYSET
              "<channel> :Channel key already set"
477    ERR_NOCHANMODES
              "<channel> :Channel doesn't support modes"

472    ERR_UNKNOWNMODE
              "<char> :is unknown mode char to me for <channel>"

324    RPL_CHANNELMODEIS
              "<channel> <mode> <mode params>"

//useless in my case
367    RPL_BANLIST
              "<channel> <banmask>"
       368    RPL_ENDOFBANLIST
              "<channel> :End of channel ban list"

         - When listing the active 'bans' for a given channel,
           a server is required to send the list back using the
           RPL_BANLIST and RPL_ENDOFBANLIST messages.  A separate
           RPL_BANLIST is sent for each active banmask.  After the
           banmasks have been listed (or if none present) a
           RPL_ENDOFBANLIST MUST be sent.

			  
Channel mode message

      Command: MODE
   Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )

   The MODE command is provided so that users may query and change the
   characteristics of a channel.  For more details on available modes
   and their uses, see "Internet Relay Chat: Channel Management" [IRC-
   CHAN].  Note that there is a maximum limit of three (3) changes per
   command for modes that take a parameter.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_KEYSET
           ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
           ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
		   
           RPL_CHANNELMODEIS
		   
           RPL_BANLIST                     RPL_ENDOFBANLIST
		   
           RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
		   
           RPL_INVITELIST                  RPL_ENDOFINVITELIST
		   
           RPL_UNIQOPIS

   The following examples are given to help understanding the syntax of
   the MODE command, but refer to modes defined in "Internet Relay Chat:
   Channel Management" [IRC-CHAN].

   Examples:

   MODE #Finnish +imI *!*@*.fi     ; Command to make #Finnish channel
                                   moderated and 'invite-only' with user
                                   with a hostname matching *.fi
                                   automatically invited.

   MODE #Finnish +o Kilroy         ; Command to give 'chanop' privileges
                                   to Kilroy on channel #Finnish.

   MODE #Finnish +v Wiz            ; Command to allow WiZ to speak on
                                   #Finnish.

   MODE #Fins -s                   ; Command to remove 'secret' flag
                                   from channel #Fins.

   MODE #42 +k oulu                ; Command to set the channel key to
                                   "oulu".

   MODE #42 -k oulu                ; Command to remove the "oulu"
                                   channel key on channel "#42".

   MODE #eu-opers +l 10            ; Command to set the limit for the
                                   number of users on channel
                                   "#eu-opers" to 10.

   :WiZ!jto@tolsun.oulu.fi MODE #eu-opers -l
                                   ; User "WiZ" removing the limit for
                                   the number of users on channel "#eu-
                                   opers".

   MODE &oulu +b                   ; Command to list ban masks set for
                                   the channel "&oulu".

   MODE &oulu +b *!*@*             ; Command to prevent all users from
                                   joining.

   MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu
                                   ; Command to prevent any user from a
                                   hostname matching *.edu from joining,
                                   except if matching *.bu.edu

   MODE #bu +be *!*@*.edu *!*@*.bu.edu
                                   ; Comment to prevent any user from a
                                   hostname matching *.edu from joining,
                                   except if matching *.bu.edu

   MODE #meditation e              ; Command to list exception masks set
                                   for the channel "#meditation".

   MODE #meditation I              ; Command to list invitations masks
                                   set for the channel "#meditation".

   MODE !12345ircd O               ; Command to ask who the channel
                                   creator for "!12345ircd" is


*/