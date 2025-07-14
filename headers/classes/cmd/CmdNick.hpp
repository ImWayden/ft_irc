/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 02:37:41 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 20:27:25 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDNICK_HPP
#define CMDNICK_HPP

#include "manager/ClientManager.hpp"
#include "ressources/r_strings.hpp"
#include "utils/CmdUtils.hpp"

#define NICK_MIN_LENGTH 1
#define NICK_MAX_LENGTH 9


#define ERRCODE_NONICKNAMEGIVEN 431
#define ERRCODE_ERRONEUSNICKNAME 432
#define ERRCODE_NICKNAMEINUSE 433

#define ERRSTRING_NONICKNAMEGIVEN " :No nickname given"
#define ERRSTRING_ERRONEUSNICKNAME " :Erroneus nickname"
#define ERRSTRING_NICKNAMEINUSE " :Nickname is already in use"


//no polymorphisme since it's fairly useless in this context, switch are faster and do not require explicit alloc and dealloc

class CmdNick {
public:
	CmdNick();
	CmdNick(ClientManager& clientManager, std::string server_password);
	~CmdNick();
	CmdNick(const CmdNick &other);
	CmdNick &operator=(const CmdNick &other);

	void execute(const CommandData &cmd);
private:
	bool checknickname(const std::string &nickname);
	ClientManager* _clientManager;
	std::string _serverpassword;
};





#endif