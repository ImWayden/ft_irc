/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PortManager.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:55:10 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:18:48 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/PortManager.hpp"


PortManager::PortManager() : _portString("0"), _port(0), _status(false) {}

//probably won't be used, but it's here for completeness
PortManager::PortManager(int port) : _portString("0"), _port(0), _status(false) {
	std::ostringstream oss;
	oss << port;
	setPort(port, oss.str());
}

PortManager::PortManager(std::string port) : _portString("0"), _port(0), _status(false) {
	std::istringstream iss(port);
	int portNumber;
	iss >> portNumber;
	if (!iss.fail() && iss.eof())
		setPort(portNumber, port);
	else {
		LogManager::logError("Invalid port number");
	}
}


PortManager::PortManager(const PortManager &other) : _port(other._port) {}

PortManager::~PortManager() {}

PortManager &PortManager::operator=(const PortManager &other) {
	if (this != &other) {
		_port = other._port;
		_portString = other._portString;
		_status = other._status;
	}
	return *this;
}

void PortManager::setPort(int port, std::string portString) {
	if (isValidPort(port)) {
		_port = port;
		_portString = portString;
		_status = true;
	} else {
		LogManager::logError("Invalid port number");
		_status = false;
	}
}

int PortManager::getPort() const {
	return _port;
}

bool PortManager::getStatus() const {
	return _status;
}

bool PortManager::isValidPort(int port) const {
	return (port >= MIN_PORT && port <= MAX_PORT);
}

const std::string& PortManager::getPortString() const {
	return _portString;
}