/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassswordManager.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:06:07 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 19:18:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "manager/PasswordManager.hpp"
#include "ressources/r_numbers.hpp"

PasswordManager::PasswordManager() : _password(""), _status(false) {}

PasswordManager::PasswordManager(const std::string &password) : _password(""), _status(false) {
	setPassword(password);
}

PasswordManager::PasswordManager(const PasswordManager &other) : _password(other._password), _status(other._status) {}
PasswordManager::~PasswordManager() {}
PasswordManager &PasswordManager::operator=(const PasswordManager &other) {
	if (this != &other) {
		_password = other._password;
		_status = other._status;
	}
	return *this;
}

void PasswordManager::setPassword(const std::string &password) {
	if (isValidPassword(password)) {
		_password = password;
		_status = true;
	} else {
		LogManager::logError("Invalid password");
		_status = false;
	}
}

const std::string &PasswordManager::getPassword() const {
	return _password;
}

bool PasswordManager::isValidPassword(const std::string &password) const {
	if (password.length() < MIN_PASSWORD_LENGTH || password.length() > MAX_PASSWORD_LENGTH)
		return false;
	for (size_t i = 0; i < password.length(); ++i) {
		char c = password[i];
		if (c <= PRINTABLE_ASCII_MIN || c > PRINTABLE_ASCII_MAX )
			return false;
	}
	return true;
}

bool PasswordManager::getStatus() const {
	return _status;
}