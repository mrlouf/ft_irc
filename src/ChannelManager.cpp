/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:28:43 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 12:17:05 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ChannelManager.hpp"
#include "../includes/ClientManager.hpp"

// Constructor and Destructor
ChannelManager::ChannelManager(ClientManager *clientManager): _clientManager(clientManager) {}

ChannelManager::~ChannelManager() {}

// Getters and Setters
const std::map<std::string, Channel> &ChannelManager::getChannelList() const { return (_channels); }

Channel* ChannelManager::getChannel(const std::string &name) {
	if (_channels.find(name) != _channels.end()) {
		return &_channels[name];
	}
	return (NULL);
}

//Methods
bool ChannelManager::createChannel(const std::string &name) {
    if (_channels.find(name) != _channels.end()) {
        return (false);
    }

    _channels[name] = Channel(name);
    return (true);
}

bool ChannelManager::removeChannel(const std::string &name) {
	if (_channels.find(name) == _channels.end()) {
        return (false);
    }
	
	_channels.erase(name);
	return (true);
}

std::vector<Channel*> ChannelManager::getClientChannels(int client_fd) {
	std::vector<Channel*> _clientChannels;
	RegisteredClient *client = _clientManager->getClientFromFd(client_fd);

	if (!client) {
		return _clientChannels;
	}

	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		if (it->second.isMember(client)) {
			_clientChannels.push_back(&(it->second));
		}
	}

	return (_clientChannels);
}

void ChannelManager::removeClientFromAllChannels(RegisteredClient *client) {
    std::vector<Channel *> channels = getClientChannels(client->getFd());

    for (size_t i = 0; i < channels.size(); i++) {
        Channel *channel = channels[i];

        if (channel->isOperator(client)) {
            if (channel->getOperators().size() == 1) {
                channel->setOperatorsToNoOps();
            } else {
                channel->removeOperator(client);
            }
        }

        channel->removeMember(client);
    }
}

// Testing method
void ChannelManager::printChannelList(int client_fd){
	if (_channels.empty()) {
		std::string response = "No available channels.\r\n";
		send(client_fd, response.c_str(), response.length(), 0);
		return;
	}

	std::ostringstream response;
	response << "Channels available:\n";

	for (std::map<std::string, Channel>::const_iterator it = _channels.begin(); it != _channels.end(); ++it) {
		const Channel& channel = it->second;
		response << " - " << channel.getName() << " - " << channel.getTopic() << "\n";

		// Operators
		response << "    - Operators: ";
		const std::vector<RegisteredClient*>& operators = channel.getOperators();
		if (operators.empty()) {
			response << "None";
		} else {
			for (size_t i = 0; i < operators.size(); i++) {
				if (i > 0) response << ", ";
				response << operators[i]->getNickname();
			}
		}
		response << "\n";

		// Members
		response << "    - Members: ";
		const std::vector<RegisteredClient*>& members = channel.getMembers();
		if (members.empty()) {
			response << "None";
		} else {
			for (size_t i = 0; i < members.size(); i++) {
				if (i > 0) response << ", ";
				response << members[i]->getNickname();
			}
		}
		response << "\r\n";
	}

	std::string responseStr = response.str();
	send(client_fd, responseStr.c_str(), responseStr.length(), 0);
}