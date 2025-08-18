/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wayden <wayden@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 01:38:01 by wayden            #+#    #+#             */
/*   Updated: 2025/08/18 14:23:37 by wayden           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "data/Channel.hpp"

Channel::Channel()
    : _name(""),
      _key(""),
      _topic(""),
      _isFull(false),
      _isInviteOnly(false),
      _hasKey(false),
      _isTopicProtected(false),
      _maxClients(0),
      _invited(),
      _bannneds(),
      _clients(),
      _operators()
{}

Channel::Channel(const std::string &name)
    : _name(name),
      _key(""),
      _topic(""),
      _isFull(false),
      _isInviteOnly(false),
      _hasKey(false),
      _isTopicProtected(false),
      _maxClients(0),
      _invited(),
      _bannneds(),
      _clients(),
      _operators()
{}

Channel::Channel(const std::string &name, const std::string &key)
    : _name(name),
      _key(key),
      _topic(""),
      _isFull(false),
      _isInviteOnly(false),
      _hasKey(!key.empty()),
      _isTopicProtected(false),
      _maxClients(0),
      _invited(),
      _bannneds(),
      _clients(),
      _operators()
{}

Channel::Channel(const Channel &other)
    : _name(other._name),
      _key(other._key),
      _topic(other._topic),
      _isFull(other._isFull),
      _isInviteOnly(other._isInviteOnly),
      _hasKey(other._hasKey),
      _isTopicProtected(other._isTopicProtected),
      _maxClients(other._maxClients),
      _invited(other._invited),
      _bannneds(other._bannneds),
      _clients(other._clients),
      _operators(other._operators)
{}

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
	if(_clients.size() == _maxClients)
		_isFull = true;
	LogManager::logServerTech(toString());
}

void Channel::addOperator(Client *client) {
	if(_clients.find(client) == _clients.end()) //check if client is in the channel
		addClient(client);
	_operators.insert(client);
}

void Channel::removeOperator(Client *client) {
	_operators.erase(client);
}

void Channel::removeClient(Client *client) {
	_clients.erase(client);
	_operators.erase(client); //do i need to check if it exists?
	client->partChannel(_name);
}

void Channel::broadcast(const std::string &message, Client* senderFd) {
	if(_clients.find(senderFd) == _clients.end() && senderFd)
	{
		return LogManager::logWarning(senderFd->getNickname() + " is not in the channel");	
	}
	
	for (std::set<Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if(senderFd != (*it))
			(*it)->addMessage_out(message);
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

bool Channel::isOperator(Client *client) const {
	return (_operators.find(client) != _operators.end());
}

bool Channel::hasKey() const {
	return _hasKey;
}

void Channel::setKey(const std::string &key) {
	_key = key;
	if(!_key.empty())
		_hasKey = true;
	else
		_hasKey = false;
}

void Channel::setName(const std::string &name) {
	_name = name;
}

void Channel::setTopic(const std::string &topic) {
	_topic = topic;
}

void Channel::setLimit(int limit) {
	_maxClients = limit;
	if(_clients.size() >= _maxClients)
		_isFull = true;
	else
		_isFull = false;
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

const std::set<Client *>& Channel::getClients() const {
	return _clients;
}

std::string Channel::getModesString() const {
    std::string modes = "+";

    if (_isInviteOnly)       modes += "i";
    if (_isTopicProtected)   modes += "t";
    if (!_key.empty())     modes += "k";
    if (_maxClients != MAX_CLIENTS) modes += "l"; 
    // mode 'o' n’apparaît pas ici (il est appliqué par utilisateur, pas globalement au channel)

    if (modes == "+") // si aucun mode n’est actif
        return "";

    return modes;
}

std::string Channel::getModeParams() const {
    std::string params;

    if (!_key.empty()) {
        if (!params.empty()) params += " ";
        params += _key;
    }
    if (_maxClients != MAX_CLIENTS) {
        if (!params.empty()) params += " ";
        std::ostringstream oss;
        oss << _maxClients;
        params += oss.str();
    }

    return params;
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

void Channel::setTopicProtected(bool isProtected) {
	_isTopicProtected = isProtected;
}

bool Channel::isTopicProtected() const {
	return _isTopicProtected;
}
std::string Channel::toString() const {
	std::ostringstream oss;
	
    oss << "=== Channel ===\n";
    oss << "Name: " << _name << "\n";
    oss << "Key: " << _key << "\n";
    oss << "Topic: " << _topic << "\n";
    oss << "isFull: " << (_isFull ? "true" : "false") << "\n";
    oss << "isInviteOnly: " << (_isInviteOnly ? "true" : "false") << "\n";
    oss << "hasKey: " << (_hasKey ? "true" : "false") << "\n";
    oss << "isTopicProtected: " << (_isTopicProtected ? "true" : "false") << "\n";
    oss << "maxClients: " << _maxClients << "\n";

    oss << "Invited clients (" << _invited.size() << "): ";
    for (std::set<Client *>::const_iterator it = _invited.begin(); it != _invited.end(); ++it) {
        if (it != _invited.begin()) oss << ", ";
        oss << (*it)->getNickname();
    }
    oss << "\n";
    oss << "Banned clients (" << _bannneds.size() << "): ";
    for (std::set<Client *>::const_iterator it = _bannneds.begin(); it != _bannneds.end(); ++it) {
        if (it != _bannneds.begin()) oss << ", ";
        oss << (*it)->getNickname();
    }
    oss << "\n";
    oss << "Clients in channel (" << _clients.size() << "): ";
    for (std::set<Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it) {
        if (it != _clients.begin()) oss << ", ";
        oss << (*it)->getNickname();
    }
    oss << "\n";
    oss << "Operators (" << _operators.size() << "): ";
    for (std::set<Client *>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
        if (it != _operators.begin()) oss << ", ";
        oss << (*it)->getNickname();
    }
    oss << "\n";
    return oss.str();
}