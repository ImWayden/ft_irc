/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IPollControl.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 20:40:18 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 22:34:12 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IPOLLCONTROL_HPP
#define IPOLLCONTROL_HPP

#include <vector>

class IPollControl {
public:
	virtual ~IPollControl() {}
	virtual void setEvent(int fd, short events) = 0;
};



#endif