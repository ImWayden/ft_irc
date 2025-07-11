/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMaker.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:43:12 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 22:55:45 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMAKER_HPP
# define COMMANDMAKER_HPP

#include <string>
#include "struct/struct.hpp"
//no canonical form since it does not make sens on a class that is not instanciable
class CommandMaker
{
public:
	static CommandData msgToCommand(Client* client, ClientMessage_t* input);
	
private:
	CommandMaker();
	~CommandMaker();

	static void parseCommand(std::string& input, CommandData& cmd);
	static uint32_t hashCommand(const std::string& cmd);
	static void findNextToken(std::string &input, std::string::size_type& pos, std::string& output);
};





#endif