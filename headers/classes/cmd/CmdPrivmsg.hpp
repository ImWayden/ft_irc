/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPrivmsg.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:14:40 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PRIVMSG_HPP
# define CMD_PRIVMSG_HPP

#include "utils/CmdUtils.hpp"

class CmdPrivmsg{
public:
	CmdPrivmsg();
	CmdPrivmsg(CmdPrivmsg const & src);
	~CmdPrivmsg();
	CmdPrivmsg & operator=(CmdPrivmsg const & rhs);
	void execute(const CommandData &cmd);
private:

	
};


#endif // CMD_PRIVMSG_HPP