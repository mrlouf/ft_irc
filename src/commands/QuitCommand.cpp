/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:28:52 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 15:10:33 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/QuitCommand.hpp"
#include "../../includes/ServerManager.hpp"
#include "../../includes/ClientManager.hpp"
#include <iostream>

QuitCommand::QuitCommand(ServerManager *serverManager, ChannelManager *channelManager) : _serverManager(serverManager), _channelManager(channelManager) {}

void QuitCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
		ClientManager *clientManager = _serverManager->getClientManager();
		RegisteredClient *client = clientManager->getClientFromFd(client_fd);

		if (!client) {
				std::cerr << "Error: Client with fd " << client_fd << " not found." << std::endl;
				return;
		}

		std::string nickname = client->getNickname();
		std::string goodbyeMsg = "Client " + nickname + " has quit.";

		if (!parsedMsg.params.empty()) {
				goodbyeMsg = parsedMsg.params[0];
				for (size_t i = 1; i < parsedMsg.params.size(); i++) {
						goodbyeMsg += " " + parsedMsg.params[i];
				}
		}

		// Create appropriate messages for the QUIT
		// This one is for direct sending to the client who is quitting
		std::string quitMessageToClient = ":" + nickname + "!~user@server QUIT :" + goodbyeMsg + "\r\n";
		
		// This is just the command part for the broadcastMessage method
		std::string quitCommand = "QUIT :" + goodbyeMsg;

		// Get channels before modifying anything
		std::vector<Channel *> channels = _channelManager->getClientChannels(client_fd);

		// Remove client from all channels before broadcasting
		_channelManager->removeClientFromAllChannels(client);

		// Now, broadcast to remaining members
		for (size_t i = 0; i < channels.size(); i++) {
				Channel *channel = channels[i];

				// Let the channel's broadcastMessage handle the formatting
				channel->broadcastMessage(quitCommand, client);

				// Send updated NAMES list to remaining users
				std::string nameReply = ":server 353 " + nickname + " = " + channel->getName() + " :";
				for (size_t j = 0; j < channel->getOperators().size(); j++) {
						nameReply += "@" + channel->getOperators()[j]->getNickname() + " ";
				}
				for (size_t j = 0; j < channel->getMembers().size(); j++) {
						nameReply += channel->getMembers()[j]->getNickname() + " ";
				}
				nameReply += "\r\n";
				
				for (size_t j = 0; j < channel->getMembers().size(); j++) {
						send(channel->getMembers()[j]->getFd(), nameReply.c_str(), nameReply.length(), 0);
				}

				// Send End of NAMES list
				std::string endOfNames = ":server 366 " + nickname + " " + channel->getName() + " :End of /NAMES list.\r\n";
				for (size_t j = 0; j < channel->getMembers().size(); j++) {
						send(channel->getMembers()[j]->getFd(), endOfNames.c_str(), endOfNames.length(), 0);
				}
		}

		// Finally, disconnect client
		_serverManager->disconnectClient(nickname, client_fd);
}