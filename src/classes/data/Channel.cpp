/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:38:01 by wayden            #+#    #+#             */
/*   Updated: 2025/07/11 23:49:30 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data/Channel.hpp"

Channel::Channel(const std::string &name, const std::string &key) : _name(name), _key(key) 
{
	_isFull = false;
	_isInviteOnly = false;
	_hasKey = false;
	_topic = "";
	if(!key.empty())
		_hasKey = true;
}

Channel::Channel(const Channel &other) : _name(other._name), _key(other._key), _isFull(other._isFull), _isInviteOnly(other._isInviteOnly)\
, _hasKey(other._hasKey), _topic(other._topic), _invited(other._invited), _clients(other._clients), _operators(other._operators){}

Channel::~Channel() {}

Channel &Channel::operator=(const Channel &other) {
	if (this != &other) {
		_name = other._name;
		_key = other._key;
		_isFull = other._isFull;
		_isInviteOnly = other._isInviteOnly;
		_hasKey = other._hasKey;
		_topic = other._topic;
		_invited = other._invited;
		_clients = other._clients;
		_operators = other._operators;
	}
	return *this;
}

void Channel::addClient(Client *client) {
	
	_clients.insert(client);
	if(_clients.size() == MAX_CLIENTS)  // need to check if max client is modddable on runtime
		_isFull = true;
}

void Channel::addOperator(Client *client) {
	if(_clients.find(client) == _clients.end()) //check if client is in the channel
		addClient(client);
	_operators.insert(client);
}

void Channel::removeClient(Client *client) {
	_clients.erase(client);
	_operators.erase(client); //do i need to check if it exists?
}

void Channel::broadcast(const std::string &message, Client* senderFd) {
	for (std::set<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (*it != senderFd) { //not certain about this need to check if the client sends the msg to himself too
			(*it)->addMessage_out(message);
		}
	}
}

bool Channel::isFull() const {
	return _isFull;
}

bool Channel::isBanned(Client *client) const {
	return (_bannneds.find(client) != _bannneds.end());
}

bool Channel::isInviteOnly() const {
	return _isInviteOnly;
}

bool Channel::isInvited(Client *client) const {
	return (_invited.find(client) != _invited.end());
}

bool Channel::hasKey() const {
	return _hasKey;
}

void Channel::setKey(const std::string &key) {
	_key = key;
}

void Channel::setName(const std::string &name) {
	_name = name;
}

void Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

std::string Channel::getKey() const {
	return _key;
}

std::string Channel::getName() const {
	return _name;
}

std::string Channel::getTopic() const {
	return _topic;
}

void Channel::Ban(Client *client) {
	_bannneds.insert(client);
	removeClient(client);
}

void Channel::setInviteOnly(bool inviteOnly) {
	_isInviteOnly = inviteOnly;
}

void Channel::Invite(Client *client) {
	_invited.insert(client);
}
