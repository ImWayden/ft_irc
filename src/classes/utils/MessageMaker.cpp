/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMaker.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:59:40 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 15:01:49 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/MessageMaker.hpp"
#include "data/Client.hpp"
#include <sstream>

ServerMessage_t MessageMaker::makePrefix(const CommandData &cmd, bool isFromClient){
	ServerMessage_t prefix;
	prefix += ':';//is it usefull need to verify compatibility with get prefix?
	if(isFromClient)
		prefix += cmd.client->getPrefix();
	else
		prefix += "ircserv";//TODO replace avec soit define soit vrai servername
	return prefix;
}


std::string MessageMaker::int_to_string(int value) {
    std::ostringstream oss;
    oss << std::setw(3) << std::setfill('0') << value;
    return oss.str();
}

ServerMessage_t MessageMaker::makeMessageCode(const CommandData &cmd, int msg_code, std::string msg) {
	ServerMessage_t messageCode;
	if(!msg.empty())
		messageCode += msg;
	else if(msg_code > 0)
		messageCode += int_to_string(msg_code);
	else
		messageCode += cmd.cmd;
	return messageCode;
}

ServerMessage_t MessageMaker::MessageGenerator(std::string sender, std::string msg_code, std::string target, std::string param) 
{
	ServerMessage_t message;
	message += ":";
	message += sender;
	message += " ";
	message += msg_code;
	message += " ";
	message += target;
	message += " ";
	if(!param.empty())
		message += param;
	message += "\r\n";
	return message;
}
