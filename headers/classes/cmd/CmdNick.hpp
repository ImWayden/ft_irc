/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdNick.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 02:37:41 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:19:46 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDNICK_HPP
#define CMDNICK_HPP

#include "manager/ClientManager.hpp"
#include "utils/CmdUtils.hpp"




#define ERRCODE_NONICKNAMEGIVEN "431"
#define ERRCODE_ERRONEUSNICKNAME "432"
#define ERRCODE_NICKNAMEINUSE "433"

#define ERRSTRING_NONICKNAMEGIVEN ":No nickname given"
#define ERRSTRING_ERRONEUSNICKNAME(nickname)  nickname + " :Erroneus nickname"
#define ERRSTRING_NICKNAMEINUSE(nickname) nickname + " :Nickname is already in use"


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