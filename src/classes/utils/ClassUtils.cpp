/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUtils.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:17:44 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:27:49 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils/CmdUtils.hpp"

bool CmdUtils::isValidChannelName(std::string name)
{
	std::string channelId;
	if (name[0] != '#' && name[0] != '+' && name[0] != '!' && name[0] != '&')
		return false;
	if(name[0] == '!')
	{
		channelId = name.substr(1, 5);
		name = name.substr(6);
		if(channelId.length() != 5 || channelId.find_first_not_of(ABNF_JOIN_CHANNELID) != std::string::npos)
			return false;
	}
	else
		name = name.substr(1);
	if(name.length() > 50 || name.find_first_of(ABNF_JOIN_NOUSECHAR) != std::string::npos)
		return false;
	return true;
}





std::map<std::string, std::string> CmdUtils::getChannelListFromData(const CommandData& cmd)
{
	std::map<std::string, std::string> channelList;

	std::vector<std::string> names = CmdUtils::split(cmd.args[0], ',');
	std::vector<std::string> keys = CmdUtils::split(cmd.args[1], ',');

	for (size_t i = 0; i < names.size(); ++i) {
		std::string name = names[i];
		std::string key = (i < keys.size()) ? keys[i] : "";
		if(!isValidChannelName(name))
		{
			//ERR_NOSUCHCHANNEL (403)
			cmd.client->addMessage_out(MessageMaker::MessageGenerator(cmd, false, ERRCODE_NOSUCHCHANNEL, ERRSTRING_NOSUCHCHANNEL(name)));
			continue;
		}
		else
			channelList.insert(std::make_pair(name, key));
	}
	return channelList;
}

std::vector<std::string> CmdUtils::split(const std::string& s, char delimiter) {
	std::vector<std::string> result;
	size_t start = 0;
	size_t end;

	while ((end = s.find(delimiter, start)) != std::string::npos) {
		result.push_back(s.substr(start, end - start));
		start = end + 1;
	}
	result.push_back(s.substr(start));
	return result;
}