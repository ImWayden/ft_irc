/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdPass.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 13:07:46 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 19:19:56 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDPASS_HPP
#define CMDPASS_HPP

#include "Cmd.hpp"

class CmdPass {
public:
	CmdPass();
	~CmdPass();
	CmdPass(const CmdPass &other);
	CmdPass &operator=(const CmdPass &other);
	void execute(const CommandData &cmd);
private:
};


#endif