/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 15:33:37 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 22:59:48 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data/Client.hpp"

Client::Client(int fd, IPollControl* pollController) : _data{fd, "", "", "", "", "", "", false, false, false, 0}, _buffer(""), _msg_received(), _msg_tosend(), _pollcontroller(pollController) {
	// Constructor initializes the client with a file descriptor and empty data
}

Client::Client(const Client &other) : _data(other._data), _buffer(other._buffer), _msg_received(other._msg_received) {}
Client::~Client() {}

Client &Client::operator=(const Client &other) {
	if (this != &other) {
		_data = other._data;
		_buffer = other._buffer;
	}
	return *this;
}


const std::string& Client::getNickname() const{
	return _data.nickname;
}

const std::string& Client::getUsername() const{
	return _data.username;
}

const std::string& Client::getHostname() const{
	return _data.host;
}

void Client::makePrefix() {
	if (_data.nickname.empty() || _data.username.empty() || _data.host.empty())
		return;
	_data.prefix = _data.nickname + "!" + _data.username + "@" + _data.host;
}

const std::string& Client::getPrefix() {
	if(_data.prefix.empty())
		makePrefix();
	return _data.prefix;
}

const std::string& Client::getPassword() const{
	return _data.password;
}

const std::vector<std::string>& Client::getChannels() const{
	return _data.channels;
}



void Client::setAddr(struct sockaddr_storage addr) {
	//placeholder need to check how addr and host are linked and see if this is really necessary
}

void Client::setNickname(const std::string &nickname) {
	_data.nickname = nickname;
}

void Client::setUsername(const std::string &username) {
	_data.username = username;
}

//depending on how address and hostname are linked i could add an overload to accept  struct sockaddr_storage addr
void Client::setHostname(const std::string &hostname) {
	_data.host = hostname;
}

void Client::setPassword(const std::string &password) {
	_data.password = password;
}

void Client::setAuthStatus(int authStatus) {
	_data.authstatus |= authStatus;
}

bool Client::isAuthenticated() const {
	if (_data.authstatus == FULLY_AUTHENTICATED)
		return true;
	return false;
}


void Client::addMessage_out(const std::string &message) {
	_msg_tosend.push_back(message);
	_pollcontroller->setEvent(_data.fd, POLLOUT | POLLIN); 
	//add way to signal pollfd should check for POLLOUT
}

std::vector<ClientMessage_t>* Client::getMsgReceived() {
	return &_msg_received;
}

std::deque<ServerMessage_t>* Client::getMsgToSend() {
	return &_msg_tosend;
}

void Client::joinChannel(const std::string &channel) {
	_data.channels.push_back(channel);
}

void Client::appendToBuffer(const std::string &data) {
	_buffer += data;
}

void Client::readSocket() {
	char recvbuffer[BUFFER_SIZE];
	ssize_t bytesRead;

	while (true) {
		bytesRead = recv(_data.fd, recvbuffer, BUFFER_SIZE, 0);

		if (bytesRead > 0) {
			appendToBuffer(std::string(recvbuffer, bytesRead));
		}
		else if (bytesRead == 0) {
			// Connexion fermée proprement par le client
			// Logique de déconnexion ici ??
			break;
		}
		else {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			} else {
				perror("recv");
				break;
			}
		}
	}
	
}

//parse the raw data partially prepare CommandData struct return true if a full new msg is received
bool Client::receiveMessages() {
	bool new_msg = false;
	readSocket();
	while(true) {
		ClientMessage_t outline;
		CommandData cmd;
		std::string::size_type pos = _buffer.find("\r\n");
    	if (pos == std::string::npos)
    	    break;
		outline = _buffer.substr(0, pos);
		_buffer.erase(0, pos + 2);
		_msg_received.push_back(outline);
		new_msg = true;
	}
	return new_msg;
}

bool Client::writeSocket(std::string& msg) {
	ssize_t bytesSent = send(_data.fd, msg.c_str(), msg.size(), 0); //need to check the man

	if (bytesSent > 0) {
		// Tout ou partie du message a été envoyé (tu peux gérer les envois partiels ici si tu veux)
		if (static_cast<size_t>(bytesSent) < msg.size()) {
			msg.erase(0, bytesSent);
			return false;
		}
		return true;
	}
	else if (bytesSent == 0) {
		// sent nothing ?? 
		return false;
	}
	else {
		switch (errno) {
			case EAGAIN:
				// Le socket est non-bloquant et ne peut pas envoyer maintenant ne devrait jamais se produire vu que l'on demande POLLOUT ?
				return false;
			case EPIPE:
			case ECONNRESET:
				// La connexion a disparu
				return false;

			default:
				//other errors
				return false;
		}
	}
}

void Client::setQuitStatus(int quit_status) {
	_quit_status |= quit_status;
}

void Client::sendMessages() {
	while(_msg_tosend.size()) 
	{
		std::string msg = _msg_tosend.front();
		if(writeSocket(msg))
			_msg_tosend.pop_front();
		else
			break;
	}
	if(!_msg_tosend.size());
	{
		_pollcontroller->setEvent(_data.fd, POLLIN);
		if(_quit_status & QUITTING)
			setQuitStatus(QUITTING_DONE);
	}

	
	return;
}

//TODO : client add setAuthStatus that take a bit mask, setPassword to sert the client data.password and isauthenticated to get authentification state fast.


// Placeholder for other client-related methods


/*
class Client {
public:

private:
	ClientData _data; // Client data structure
	std::vector<ClientMessage_t> _msg_received; // List of messages from the client
	std::deque<ServerMessage_t> _msg_tosend; // List of messages to write back to the client
	//bool is_getting_pinged;
};
*/