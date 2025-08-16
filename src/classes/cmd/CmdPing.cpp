/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:04:21 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:57:00 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPing.hpp"
#include "data/Client.hpp"

CmdPing::CmdPing() {}
CmdPing::CmdPing(std::string servername) : _servername(servername) {}
CmdPing::CmdPing(const CmdPing &other) : _servername(other._servername) {}
CmdPing::~CmdPing() {}

CmdPing CmdPing::operator=(const CmdPing &other) {
	_servername = other._servername;
	return (*this);
}

void	CmdPing::execute(const CommandData &cmd) {
	cmd.client->addMessage_out(MessageMaker::MessageGenerator(SERVERNAME, "PONG", _servername));
}