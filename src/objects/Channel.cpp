/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:13:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 14:03:49 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/objects/Channel.hpp"

// Constructor and Destructor
Channel::Channel() {}

Channel::Channel(const std::string &name)
: _name(name), _topic(""), _password(""), _userLimit(-1), _inviteOnlySet(false), _topicRestrictedSet(true) {}

Channel::~Channel() {}

// Getters and Setters
const std::string &Channel::getName() const { return (_name);}
const std::string &Channel::getTopic() const { return (_topic);}
const std::vector<RegisteredClient*> &Channel::getMembers() const { return (_members);}
const std::vector<RegisteredClient*> &Channel::getOperators() const { return (_operators);}

void Channel::setTopic(const std::string &topic) { _topic = topic; }

void Channel::setName(const std::string &name) { _name = name; }

// Methods
bool Channel::isMember(RegisteredClient *client) {
	for (std::vector<RegisteredClient*>::iterator it = _members.begin(); it != _members.end(); it++) {
		if ((*it) == client) {
			return (true);
		}
	}

	for (std::vector<RegisteredClient*>::iterator it = _operators.begin(); it != _operators.end(); it++) {
		if ((*it) == client) {
			return (true);
		}
	}

	return (false);
}

bool Channel::isOperator(RegisteredClient *client) {
    for (std::vector<RegisteredClient*>::iterator it = _operators.begin(); it != _operators.end(); it++) {
        if ((*it)->getNickname() == client->getNickname()) {
            return (true);
        }
    }
    return (false);
}

bool Channel::addMember(RegisteredClient *client) {
	std::vector<RegisteredClient*>::iterator it = std::find(_members.begin(), _members.end(), client);
	if (it == _members.end()) {
		_members.push_back(client);
		return (true);
	}

	return (false);
}

bool Channel::addOperator(RegisteredClient *oper) {
	std::vector<RegisteredClient*>::iterator it = std::find(_operators.begin(), _operators.end(), oper);
	if (it == _operators.end()) {
		_operators.push_back(oper);
		return (true);
	}

	return (false);
}

bool Channel::removeMember(RegisteredClient *client) {
	std::vector<RegisteredClient*>::iterator it = std::find(_members.begin(), _members.end(), client);
	if (it != _members.end()) {
		_members.erase(it);
		return (true);
	}

	return (false);
}

bool Channel::removeOperator(RegisteredClient *oper) {
	std::vector<RegisteredClient*>::iterator it = std::find(_operators.begin(), _operators.end(), oper);
	if (it != _operators.end()) {
		_operators.erase(it);
		return (true);
	}

	return (false);
}

void Channel::setOperatorsToNoOps() {
    _operators.clear();
}

void Channel::broadcastMessage(const std::string& message, RegisteredClient* sender) {
	std::string formattedMessage = ":" + sender->getNickname() + " " + message + "\r\n";
	
	for (std::vector<RegisteredClient*>::iterator it = _members.begin(); it != _members.end(); ++it) {
		if (*it != sender) {
			int client_fd = (*it)->getFd();
			send(client_fd, formattedMessage.c_str(), formattedMessage.length(), 0);
		}
	}
}

// MODE related methods
void Channel::setMode(char mode, bool enable) {
	if (enable) {
		_modes.insert(mode);
	} else {
		_modes.erase(mode);
	}
}

void Channel::setPassword(const std::string &password) {
	if (!password.empty()) {
		_password = password;
		setMode('k', true);
		_passwordSet = true;
	}
}

void Channel::setUserLimit(int limit) {
	if (limit <= INT_MAX && limit >= INT_MIN) {
		_userLimit = limit;
		setMode('l', true);
		_userLimitSet = true;
	}
}

void Channel::setInviteOnly() {
	setMode('i', true);
	_inviteOnlySet = true;
}

void Channel::setTopicRestriction() {
	setMode('t', true);
	_topicRestrictedSet = true;
}

bool Channel::isFull() {
	if (_members.size() >= _userLimit) {
		return (true);
	}
	return (false);
}

bool Channel::isInviteOnly() {
	return (_inviteOnlySet);
}
bool Channel::isTopicRestricted() {
	return (_topicRestrictedSet);
}

bool Channel::hasMode(char mode) const {
	return _modes.find(mode) != _modes.end();
}

void Channel::clearPassword() {
	_password = "";
	setMode('k', false);
	_passwordSet = false;
}

void Channel::clearUserLimit() {
	_userLimit = 0;
	setMode('l', false);
	_userLimitSet = false;
}

void Channel::clearInviteOnly() {
	setMode('i', false);
	_inviteOnlySet = false;
}

void Channel::clearTopicRestriction() {
	setMode('t', false);
	_topicRestrictedSet = false;
}

std::string Channel::getModeString() const {
	std::string modeString = "+";
	if (_passwordSet) modeString += "k";
	if (_userLimitSet) modeString += "l";
	return modeString;
}

bool Channel::canJoin(const RegisteredClient *client, const std::string &password) const{
	// TODO: check ban and invitation list when those are implemented;
	(void)client;
	if (hasMode('k') && password != _password) {
		return (false);
	}
	if (hasMode('l') && _members.size() >= _userLimit) {
		return (false);
	}
	return (true);
}