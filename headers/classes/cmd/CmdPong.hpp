/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPong.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 00:52:29 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PONG_HPP
# define CMD_PONG_HPP

#include "utils/CmdUtils.hpp"

class CmdPong {
public:
	CmdPong();
	~CmdPong();
	CmdPong(const CmdPong &other);
	CmdPong &operator=(const CmdPong &other);
	void execute(const CommandData &cmd);
private:
	
};



#endif