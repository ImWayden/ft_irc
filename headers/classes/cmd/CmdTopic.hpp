/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CmdTopic.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 01:29:16 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 23:37:59 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMDTOPIC_HPP
# define CMDTOPIC_HPP

#include "utils/CmdUtils.hpp"

class CmdTopic {
public:
	CmdTopic();
	~CmdTopic();
	CmdTopic(CmdTopic const & src);
	CmdTopic & operator=(CmdTopic const & rhs);
	void execute(const CommandData &cmd);
};

#endif