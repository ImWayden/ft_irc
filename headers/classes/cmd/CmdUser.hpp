/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdUser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 20:20:43 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDUSER_HPP
# define CMDUSER_HPP

#include "utils/CmdUtils.hpp"

#define R_ABNF_USER_NOUSECHAR "\0\r\n @"
#define NICK_MIN_LENGTH 9 // need to recheck if i need it or not

class CmdUser {
public:
	CmdUser();
	~CmdUser();
	CmdUser(const CmdUser &other);
	CmdUser &operator=(const CmdUser &other);
	void execute(const CommandData &cmd);
private:
	bool checkusername(const std::string &username);
};



#endif