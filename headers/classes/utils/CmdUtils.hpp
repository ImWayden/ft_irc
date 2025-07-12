/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUtils.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:15:45 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:28:43 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDUTILS_HPP
# define CMDUTILS_HPP

#include <string>
#include <vector>
#include <map>
#include "utils/MessageMaker.hpp"
#include "struct/struct.hpp"

#define ERRCODE_NEEDMOREPARAMS 461
#define ERRSTRING_NEEDMOREPARAMS(command) command + " :Not enough parameters"

#define ERRCODE_NOSUCHCHANNEL 403
#define ERRSTRING_NOSUCHCHANNEL(channel) channel + " :No such channel"

#define ERRCODE_BADCHANMASK 476
#define ERRSTRING_BADCHANMASK(channel) channel + " :Bad Channel Mask"

#define ERRCODE_NOTONCHANNEL 442
#define ERRSTRING_NOTONCHANNEL(channel) channel + " :You're not on that channel"

#define ERRCODE_CHANOPRIVSNEEDED 482
#define ERRSTRING_CHANOPRIVSNEEDED(channel) channel + " :You're not channel operator"

#define R_ABNF_DIGIT "0123456789"
#define R_ABNF_ALPHA_CAP "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define R_ABNF_ALPHA "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define R_ABNF_SPECIAL "[]\\`_^{|}"
#define R_ABNF_DASH "-"
#define R_ABNF_NICKNAME R_ABNF_DIGIT R_ABNF_ALPHA R_ABNF_SPECIAL R_ABNF_DASH
#define ABNF_JOIN_NOUSECHAR "\x00\x07\x0A\x0D\x20\x2C\x3A"
#define ABNF_JOIN_CHANNELID R_ABNF_ALPHA_CAP R_ABNF_DIGIT
#define R_ABNF_USER_NOUSECHAR "\0\r\n @"

class CmdUtils {
public:
	static bool isValidChannelName(std::string name);
	static std::map<std::string, std::string> CmdUtils::getChannelListFromData(const CommandData& cmd);
	static std::vector<std::string> split(const std::string& s, char delimiter);
private:
};


#endif