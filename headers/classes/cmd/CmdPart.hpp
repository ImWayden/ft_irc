/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPart.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:11:13 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDPART_HPP
#define CMDPART_HPP

#include "utils/CmdUtils.hpp"


class CmdPart {
public:
	CmdPart();
	CmdPart(CmdPart const & src);
	~CmdPart();
	CmdPart & operator=(CmdPart const & rhs);
	void execute(const CommandData &cmd);
private:

};

//TODO  check PART Documentation 

#endif