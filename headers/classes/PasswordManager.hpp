/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PasswordManager.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 16:01:35 by wayden            #+#    #+#             */
/*   Updated: 2025/07/01 16:04:20 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASSWORDMANAGER_HPP
# define PASSWORDMANAGER_HPP

#include "includes.hpp"

#define MIN_PASSWORD_LENGTH 8
#define MAX_PASSWORD_LENGTH 64

class PasswordManager
{
public:
	PasswordManager();
	PasswordManager(const std::string &password);
	PasswordManager(const PasswordManager &other);
	
	~PasswordManager();
	PasswordManager &operator=(const PasswordManager &other);
	
	void setPassword(const std::string &password);
	const std::string &getPassword() const;
private:
	bool isValidPassword(const std::string &password) const;
	std::string _password;
};

#endif // PASSWORDMANAGER_HPP