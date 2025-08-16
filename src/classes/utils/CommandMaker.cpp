/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandMaker.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 03:58:40 by wayden            #+#    #+#             */
/*   Updated: 2025/08/14 12:51:33 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/CommandMaker.hpp"
#include <iostream>

uint32_t CommandMaker::hashCommand(const std::string &cmd) {
	if (cmd.size() < 4)
        return 0xFFFFFFFF;
    uint32_t hash = 0;
    for (size_t i = 0; i < 4; ++i) {
		hash <<= 8;
        hash |= static_cast<unsigned char>(cmd[i]);
    }
    return hash;
}

void CommandMaker::findNextToken(std::string &input, std::string::size_type& pos, std::string& output)
{
	pos = input.find(' ');
	if (pos != std::string::npos) {
		output = input.substr(0, pos); // extrait le token
		input.erase(0, pos + 1); // retire le token et l'espace de la chaîne
		pos = 0;
	}
	else 
	{
        output = input;
        input.clear();
        pos = std::string::npos;
    }
}

void CommandMaker::parseCommand(std::string& input, CommandData &cmd) {
	std::string::size_type pos = 0;
	std::string arg;
	if(input[0] == ':') {
		input.erase(0, 1); // Retire le préfixe de la chaîne, mais est ce vraiment la bonne idee ? je pourria en avoir besoin pour transmettre le msg a un channel par exemple
		findNextToken(input, pos, cmd.prefix);
	}
	findNextToken(input, pos, cmd.cmd);
	while(pos != std::string::npos) {
		if(input[pos] == ':') {
			// Si l'argument commence par ':', on prend le reste de la chaîne comme un seul argument
			cmd.args.push_back(input.substr(pos + 1)); // extrait l'argument
			break; // On sort de la boucle car on a tout pris
		}
		findNextToken(input, pos, arg);
		if(!arg.empty())
			cmd.args.push_back(arg);
	}
}

CommandData CommandMaker::msgToCommand(Client* client, ClientMessage_t* input)
{
	CommandData cmd;            // Appelle le constructeur par défaut
	cmd.client = client;
	parseCommand(*input, cmd);  // Remplit les autres champs
	cmd.hash = hashCommand(cmd.cmd);
	return cmd;
}