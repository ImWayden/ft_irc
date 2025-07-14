/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 00:59:57 by wayden            #+#    #+#             */
/*   Updated: 2025/07/14 17:33:27 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <deque>
#include <sys/socket.h>
#include "struct/struct.hpp"
#include "ressources/r_numbers.hpp"
#include "interfaces/IPollControl.hpp"


#define USER_RECEIVED 0b001
#define NICK_RECEIVED 0b010
#define FULLY_AUTHENTICATED 0b11
#define PASSWORD_RECEIVED 0b100 // the RFC says Password received is not part of auth, the password validity should be check once it tries to auth after receiving bot nick and user  

#define QUITTING 0b001
#define QUITTING_DONE 0b010

#define PING_WAIT 0b000
#define PING_SENT 0b001
#define PONG_RECEIVED 0b010


#define BUFFER_SIZE 1024 // Define a buffer size for reading data

class Client {
public:
	Client(int fd);
	Client(int fd, IPollControl* pollController); //to add
	Client(pollfd *fd); // loses a bit of responsibility separation between Client and pollfdmanager since client will be able to mod the pollfd is it the good choice ?
	Client(const Client &other);
	~Client();

	Client &operator=(const Client &other);

	const int getFd() const;
	const std::string& getNickname() const;
	const std::string& getUsername() const;
	const std::string& getHostname() const;
	const std::string& getPrefix();
	const std::string& getPassword() const;
	const std::set<std::string> &getChannels() const;
	const std::string& getServerName() const;
	const int getQuitStatus() const;
	
	void setAddr(struct sockaddr_storage addr);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setHostname(const std::string &hostname);
	void setPassword(const std::string &password);
	void setAuthStatus(int authStatus);
	void setQuitStatus(int quit_status);
	void setPingStatus(int ping_status);
	
	std::vector<ClientMessage_t>* Client::getMsgReceived();
	std::deque<ServerMessage_t>* getMsgToSend();
	void appendToBuffer(const std::string &data);
	bool receiveMessages();
	bool isAuthenticated() const;
	
	void addMessage_out(const std::string &message);
	void sendMessages();
	void joinChannel(const std::string &channel);
	//void leaveChannel(const std::string &channel);
	//void leaveAllChannel(const std::string &channel);
	// Placeholder for other client-related methods
private:
	ClientData _data; // Client data structure
	IPollControl *_pollcontroller;
	void readSocket();
	void writeSocket(std::string& msg);
	
	void makePrefix();

	std::string _buffer; // Buffer for incoming data
	std::vector<ClientMessage_t> _msg_received; // List of messages from the client
	std::deque<ServerMessage_t> _msg_tosend; // List of messages to write back to the client
	std::string _buffer_tosend;
	char _quit_status;
	char _auth_status;
	char _ping_status;
	//bool is_getting_pinged;
};

#endif	// CLIENT_HPP


//TODO : client add setAuthStatus that take a bit mask, setPassword to sert the client data.password and isauthenticated to get authentification state fast.