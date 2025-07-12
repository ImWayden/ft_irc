/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:41:12 by wayden            #+#    #+#             */
/*   Updated: 2025/07/12 21:28:23 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <string>
#include <vector>
#include <poll.h>
#include <sys/socket.h>
#include "classes/data/Client.hpp"
#include "classes/data/Channel.hpp"

struct newClient {
	struct pollfd client_fd; // File descriptor for the client
	struct sockaddr_storage addr; // Address of the client
};

struct ClientData {
	int fd; // File descriptor for the client
	std::string nickname; // Nickname of the client
	std::string username; // Username of the client
	std::string servername; // Server name of the client
	std::string password; //password given by the client
	std::string host; // Host of the client
	std::string prefix; // Prefix of the client
	bool hasbeenwelcomed; // Whether the client has been welcomed
	bool isauthenticated; // Authentication status of the client
	bool isoperator; // Operator status of the client
	char authstatus; // bitmask representing authentication status (has given password, has given username, has given nickname)
	std::set<std::string> channels; // List of channels joined by the client
};

struct CommandData {
	Client* client; // Client data associated with the command
	std::string prefix; // Prefix for the command (e.g., server name, nickname)
	std::string cmd; // Command name
	std::vector<std::string> args; // Arguments for the command
	uint32_t hash;
};

typedef std::string ServerMessage_t;
typedef std::string ClientMessage_t;

#endif // STRUCT_HPP