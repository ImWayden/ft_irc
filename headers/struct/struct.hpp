/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 17:41:12 by wayden            #+#    #+#             */
/*   Updated: 2025/08/16 20:23:18 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef S_STRUCT_HPP
#define S_STRUCT_HPP

#include <sstream>
#include <string>
#include <vector>
#include <poll.h>
#include <set>
#include <sys/socket.h>
#include <stdint.h>

class Client;
struct newClient {
	struct pollfd client_fd;
	struct sockaddr_storage addr;
};

struct ClientData {
	int fd;
	std::string nickname;
	std::string username; 
	std::string servername; 
	std::string password; 
	std::string host; 
	std::string prefix;
	bool hasbeenwelcomed; 
	bool isauthenticated; 
	bool isoperator;
	char authstatus; 
	std::set<std::string> channels; 
	ClientData(int fd_init = -1,
               const std::string& nick = "",
               const std::string& user = "",
               const std::string& serv = "",
               const std::string& pass = "",
               const std::string& h = "",
               const std::string& pref = "",
               bool welcomed = false,
               bool authenticated = false,
               bool oper = false,
               char authstat = 0)
      : fd(fd_init),
        nickname(nick),
        username(user),
        servername(serv),
        password(pass),
        host(h),
        prefix(pref),
        hasbeenwelcomed(welcomed),
        isauthenticated(authenticated),
        isoperator(oper),
        authstatus(authstat),
        channels() 
    {}
	std::string toString() const {
        std::ostringstream oss;
        oss << "ClientData:\n";
        oss << "  fd=" << fd << "\n";
        oss << "  nickname=" << nickname << "\n";
        oss << "  username=" << username << "\n";
        oss << "  servername=" << servername << "\n";
        oss << "  password=" << password << "\n";
        oss << "  host=" << host << "\n";
        oss << "  prefix=" << prefix << "\n";
        oss << "  hasbeenwelcomed=" << (hasbeenwelcomed ? "true" : "false") << "\n";
        oss << "  isauthenticated=" << (isauthenticated ? "true" : "false") << "\n";
        oss << "  isoperator=" << (isoperator ? "true" : "false") << "\n";
        oss << "  authstatus=" << static_cast<int>(authstatus) << "\n";

        oss << "  channels={";
        for (std::set<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
            if (it != channels.begin()) oss << ", ";
            oss << *it;
        }
        oss << "}\n";
        return oss.str();
    }
};

struct CommandData {
	Client* client;
	std::string prefix;
	std::string cmd;
	std::vector<std::string> args;
	uint32_t hash;

	CommandData()
		: client(NULL), prefix(""), cmd(""), args(), hash(0) {}
	std::string toString() const;
};

typedef std::string ServerMessage_t;
typedef std::string ClientMessage_t;

#endif // STRUCT_HPP