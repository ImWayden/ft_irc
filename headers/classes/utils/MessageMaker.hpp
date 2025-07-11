/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageMaker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:45:06 by wayden            #+#    #+#             */
/*   Updated: 2025/07/09 03:48:51 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGEMAKER_HPP
#define MESSAGEMAKER_HPP

#include "struct/struct.hpp"
#include <iomanip>

//no canonical form since it does not make sens on a class that is not instanciable
class MessageMaker {
public:
	static ServerMessage_t MessageGenerator(const CommandData &cmd, bool isFromClient, int msg_code, std::string param);
private:
	MessageMaker();
	~MessageMaker();
	static ServerMessage_t makeMessageCode(const CommandData &cmd, int msg_code);
	static ServerMessage_t makePrefix(const CommandData &cmd, bool isFromClient);
	static std::string int_to_string(int value);
};


#endif