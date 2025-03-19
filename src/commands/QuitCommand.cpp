/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:28:52 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 18:33:34 by hmunoz-g         ###   ########.fr       */
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

    // Format the QUIT message as per RFC 2812
    std::string quitMessage = ":" + nickname + " QUIT :" + goodbyeMsg + "\r\n";

    // Notify all channels the client is part of
    std::vector<Channel *> channels = _channelManager->getClientChannels(client_fd);
    for (size_t i = 0; i < channels.size(); i++) {
        channels[i]->broadcastMessage(quitMessage, client);
        channels[i]->removeMember(client);
    }


    // Remove client from server
    _serverManager->disconnectClient(nickname, client_fd);
}