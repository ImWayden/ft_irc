/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PortManager.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:48:33 by wayden            #+#    #+#             */
/*   Updated: 2025/07/03 23:54:57 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PORTMANAGER_HPP
# define PORTMANAGER_HPP

#define MIN_PORT 1
#define MAX_PORT 65535

#include <string>
#include <sstream>

class PortManager
{
public:
	PortManager();
	PortManager(int port);
	PortManager(std::string port);
	PortManager(const PortManager &other);
	
	~PortManager();
	PortManager &operator=(const PortManager &other);
	void setPort(int port, std::string portString);
	int getPort() const;
	std::string getPortString() const;
private:
	bool isValidPort(int port) const;
	std::string _portString;
	int _port;
};

#endif // PORTMANAGER_HPP