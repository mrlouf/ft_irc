/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:28:52 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 16:48:19 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/QuitCommand.hpp"
#include "../../includes/ServerManager.hpp"
#include "../../includes/ClientManager.hpp"
#include <iostream>

//TODO: make the custom messages be passed to channels, not mirrored back to client. 

QuitCommand::QuitCommand(ServerManager* serverManager) : _serverManager(serverManager) {}

void QuitCommand::executeCommand(int client_fd, const ParsedMessage &parsedMsg) {
    std::string goodbye;

    if (!parsedMsg.params.empty()) {
        //goodbye = parsedMsg.params[0];
        for(std::vector<std::string>::const_iterator it = parsedMsg.params.begin(); it != parsedMsg.params.end(); it++) {
            goodbye += *it;
            goodbye += " ";
        }
    } else {
        goodbye = "Goodbye! Disconnecting from server.\n";
    }

    send(client_fd, goodbye.c_str(), goodbye.length(), 0);
    std::cout << "Client " << client_fd << " has quit." << std::endl;

    ClientManager *clientManager = _serverManager->getClientManager();
    RegisteredClient *client = clientManager->getClientFromFd(client_fd);
    
    if (client) {
        std::string nickname = client->getNickname();
        _serverManager->disconnectClient(nickname, client_fd);
    } else {
        std::cerr << "Error: Client with fd " << client_fd << " not found." << std::endl;
    }
}