/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPong.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:53:42 by wayden            #+#    #+#             */
/*   Updated: 2025/08/05 04:56:12 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPong.hpp"
#include "data/Client.hpp"

CmdPong::CmdPong() {}

CmdPong::~CmdPong() {}

CmdPong::CmdPong(const CmdPong &other) {
	*this = other;
}

CmdPong &CmdPong::operator=(const CmdPong &other) {
	(void)other;
	return *this;
}

void CmdPong::execute(const CommandData &cmd) 
{
	cmd.client->setPingStatus(PONG_RECEIVED);
}
