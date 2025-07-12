/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdMode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:32:40 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/CmdUtils.hpp"

#ifndef CMDMODE_HPP
# define CMDMODE_HPP

class CmdMode
{
public:
	CmdMode();
	~CmdMode();
	CmdMode(CmdMode const & src);
	CmdMode & operator=(CmdMode const & rhs);
	void execute(const CommandData & cmd);
};

#endif