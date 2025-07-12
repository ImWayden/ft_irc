/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:33:55 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 01:34:42 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdMode.hpp"

CmdMode::CmdMode() {}

CmdMode::~CmdMode() {}

CmdMode &CmdMode::operator=(const CmdMode &rhs) {
	(void)rhs;
	return *this;
}

CmdMode::CmdMode(const CmdMode &src) { (void)src; }

void CmdMode::execute(const CommandData &cmd) {}