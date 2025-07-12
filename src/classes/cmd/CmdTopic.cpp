/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:31:17 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 01:38:03 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdTopic.hpp"

CmdTopic::CmdTopic() {}

CmdTopic::~CmdTopic() {}

CmdTopic::CmdTopic(const CmdTopic &other) {
	(void)other;
}

CmdTopic &CmdTopic::operator=(const CmdTopic &other) {
	(void)other;
	return (*this);
}

void CmdTopic::execute(const CommandData &cmd) {}