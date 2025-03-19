/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:33:55 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 17:51:58 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/JoinCommand.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/ClientManager.hpp"

// Message macros
const std::string JoinCommand::CREATIONSCCS = "Channel created with name ";
const std::string JoinCommand::JOINEDMEMBERSCCS = "Joined as member to channel ";
const std::string JoinCommand::JOINEDOPERATORSCCS = "Joined as operator to channel ";
const std::string JoinCommand::CREATIONERR = "Failed to create channel ";
const std::string JoinCommand::JOINEDOPERATORERR = "Failed to join as operator to channel ";
const std::string JoinCommand::JOINEDMEMBERERR = "Failed to join as member to channel ";

// Constructors and destructor
JoinCommand::JoinCommand() {}

JoinCommand::JoinCommand(ChannelManager *channelManager, ClientManager *clientManager) : _channelManager(channelManager), _clientManager(clientManager) {}

JoinCommand::~JoinCommand() {}

//Methods
void JoinCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
	std::string name = parsedMsg.params[0];
	RegisteredClient *client = _clientManager->getClientFromFd(client_fd);

	if (_channelManager->getChannelList().find(name) == _channelManager->getChannelList().end()) {
		if (_channelManager->createChannel(name)) {
			std::string creationGoodOutput = CREATIONSCCS + name + "\r\n";
			send(client_fd, creationGoodOutput.c_str(), creationGoodOutput.length(), 0);
		} else {
			std::string creationBadOutput = CREATIONERR + name + "\r\n";
			send(client_fd, creationBadOutput.c_str(), creationBadOutput.length(), 0);
		}
	}

	Channel *channel = _channelManager->getChannel(name);

	if (channel->getOperators().empty()) {
		if (channel->addOperator(client)) {
			std::string operatorGoodMessage = JOINEDOPERATORSCCS + name + "\r\n";
			send(client_fd, operatorGoodMessage.c_str(), operatorGoodMessage.length(), 0);
			broadcast(channel, client_fd);
		} else {
			std::string operatorBadMessage = JOINEDOPERATORERR + name + "\r\n";
			send(client_fd, operatorBadMessage.c_str(), operatorBadMessage.length(), 0);
		}
	} else {
		if (channel->addMember(client)) {
			std::string memberGoodMessage = JOINEDMEMBERSCCS + name + "\r\n";
			send(client_fd, memberGoodMessage.c_str(), memberGoodMessage.length(), 0);
			broadcast(channel, client_fd);
		} else {
			std::string memberBadMessage = JOINEDMEMBERERR + name + "\r\n";
			send(client_fd, memberBadMessage.c_str(), memberBadMessage.length(), 0);
		}
	}
}

void JoinCommand::broadcast(Channel *channel, int client_fd){
    std::vector<RegisteredClient*> members = channel->getMembers();
    std::vector<RegisteredClient*> operators = channel->getOperators();
    
    // Broadcasting to all members
    for (size_t i = 0; i < members.size(); ++i) {
        RegisteredClient* member = members[i];
        if (member->getFd() != client_fd) {
            std::string message = member->getNickname() + " joined the channel!\r\n";
            send(member->getFd(), message.c_str(), message.length(), 0);
        }
    }
    
    // Broadcasting to all operators
    for (size_t i = 0; i < operators.size(); ++i) {
        RegisteredClient* operatorClient = operators[i];
        if (operatorClient->getFd() != client_fd) {
            std::string message = operatorClient->getNickname() + " joined the channel!\r\n";
            send(operatorClient->getFd(), message.c_str(), message.length(), 0);
        }
    }
}