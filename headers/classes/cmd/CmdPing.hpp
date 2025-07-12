/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:01:15 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_PING_HPP
# define CMD_PING_HPP

#include "utils/CmdUtils.hpp"

class CmdPing{
public:
	CmdPing();
	CmdPing(const CmdPing &other);
	CmdPing(std::string servername);
	~CmdPing();
	CmdPing operator=(const CmdPing &other);
	void	execute(const CommandData &cmd);
private:
	std::string _servername;
};

#endif