/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMaker.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:59:40 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 22:24:12 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/MessageMaker.hpp"


ServerMessage_t MessageMaker::makePrefix(const CommandData &cmd, bool isFromClient){
	ServerMessage_t prefix;
	prefix += ':';//is it usefull need to verify compatibility with get prefix?
	if(isFromClient)
	{
		prefix += cmd.client->getPrefix();
	}
	else
	{
		prefix += cmd.client->getServerName(); //
	}	
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

ServerMessage_t MessageMaker::MessageGenerator(const CommandData &cmd, bool isFromClient, int msg_code, std::string param, std::string msg_string) 
{
	ServerMessage_t message;
	message += makePrefix(cmd, isFromClient);
	message += " ";
	message += makeMessageCode(cmd, msg_code, msg_string);
	message += " ";
	message += param;
	message += " ";
	message += "\r\n";
	return message;
}