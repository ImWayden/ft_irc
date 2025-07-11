/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassswordManager.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:06:07 by wayden            #+#    #+#             */
/*   Updated: 2025/07/09 02:16:01 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PasswordManager.hpp"
#include "ressources/r_numbers.hpp"

PasswordManager::PasswordManager() : _password("") {}

PasswordManager::PasswordManager(const std::string &password) : _password("") {
	setPassword(password);
}

PasswordManager::PasswordManager(const PasswordManager &other) : _password(other._password) {}
PasswordManager::~PasswordManager() {}
PasswordManager &PasswordManager::operator=(const PasswordManager &other) {
	if (this != &other) {
		_password = other._password;
	}
	return *this;
}

void PasswordManager::setPassword(const std::string &password) {
	if (isValidPassword(password)) {
		_password = password;
	} else {
		//throw std::invalid_argument("Invalid password"); Placeholder for exception handling
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