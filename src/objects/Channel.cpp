/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:13:38 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 12:24:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/objects/Channel.hpp"

// Constructor and Destructor
Channel::Channel() {}

Channel::Channel(const std::string &name): _name(name) {}

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

bool Channel::RemoveOperator(RegisteredClient *oper) {
	std::vector<RegisteredClient*>::iterator it = std::find(_operators.begin(), _operators.end(), oper);
	if (it != _operators.end()) {
		_operators.erase(it);
		return (true);
	}

	return (false);
}

void Channel::broadcastMessage(const std::string& message, RegisteredClient* sender) {
	std::string formattedMessage = ":" + sender->getNickname() + " " + message + "\r\n";
	
	for (std::vector<RegisteredClient*>::iterator it = _members.begin(); it != _members.end(); ++it) {
		if (*it != sender) {  // Avoid sending message back to sender
			int client_fd = (*it)->getFd();
			send(client_fd, formattedMessage.c_str(), formattedMessage.length(), 0);
		}
	}
}
