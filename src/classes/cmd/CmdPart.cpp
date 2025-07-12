/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:12:53 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 01:13:43 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmd/CmdPart.hpp"

CmdPart::CmdPart() {}

CmdPart::CmdPart(CmdPart const & src) { (void)src; }

CmdPart::~CmdPart() {}

CmdPart & CmdPart::operator=(CmdPart const & rhs) { (void)rhs; return *this; }

void CmdPart::execute(const CommandData &cmd) {}

