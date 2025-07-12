/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMaker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:45:06 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:38:54 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEMAKER_HPP
#define MESSAGEMAKER_HPP

#include "struct/struct.hpp"
#include <iomanip>

#define PONG -19
//no canonical form since it does not make sens on a class that is not instanciable
class MessageMaker {
public:
	static ServerMessage_t MessageGenerator(const CommandData &cmd, bool isFromClient, int msg_code, std::string param, std::string msg_string = "");
private:
	static ServerMessage_t makeMessageCode(const CommandData &cmd, int msg_code, std::string msg = "");
	static ServerMessage_t makePrefix(const CommandData &cmd, bool isFromClient);
	static std::string int_to_string(int value);
};


#endif