/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PortManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:55:10 by wayden            #+#    #+#             */
/*   Updated: 2025/07/09 14:52:56 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/PortManager.hpp"


PortManager::PortManager() : _port(0), _portString("0") {}

//probably won't be used, but it's here for completeness
PortManager::PortManager(int port) : _port(0), _portString("0") {
	std::ostringstream oss;
	oss << port;
	setPort(port, oss.str());
}

PortManager::PortManager(std::string port) : _port(0), _portString("0") {
	std::istringstream iss(port);
	int portNumber;
	iss >> portNumber;
	if (!iss.fail() && iss.eof())
		setPort(portNumber, port);
	else {
		//throw std::invalid_argument("Invalid port string"); Placeholder for exception handling
	}
}


PortManager::PortManager(const PortManager &other) : _port(other._port) {}

PortManager::~PortManager() {}

PortManager &PortManager::operator=(const PortManager &other) {
	if (this != &other) {
		_port = other._port;
	}
	return *this;
}

void PortManager::setPort(int port, std::string portString) {
	if (isValidPort(port)) {
		_port = port;
		_portString = portString;
	} else {
		//throw std::invalid_argument("Invalid port number"); Placeholder for exception handling
	}
}

int PortManager::getPort() const {
	return _port;
}

bool PortManager::isValidPort(int port) const {
	return (port >= MIN_PORT && port <= MAX_PORT);
}

std::string PortManager::getPortString() const {
	return _portString;
}