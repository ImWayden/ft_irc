/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMaker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:45:06 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 10:01:24 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEMAKER_HPP
#define MESSAGEMAKER_HPP

#include "struct/struct.hpp"
#include <iomanip>

#define SERVERNAME "ircserv"
//no canonical form since it does not make sens on a class that is not instanciable
class MessageMaker {
public:
	static ServerMessage_t MessageGenerator(std::string sender, std::string msg_code, std::string target, std::string param = "");
private:
	static ServerMessage_t makeMessageCode(const CommandData &cmd, int msg_code, std::string msg = "");
	static ServerMessage_t makePrefix(const CommandData &cmd, bool isFromClient);
	static std::string maketarget(bool isFromClient, const CommandData &cmd, const std::string &target);
	static std::string int_to_string(int value);
};


#endif