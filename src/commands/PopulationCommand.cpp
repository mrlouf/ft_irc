/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PopulationCommand.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:16:21 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 16:55:39 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/PopulationCommand.hpp"
#include "../../includes/ClientManager.hpp"

// Constructor that takes a ClientManager reference
PopulationCommand::PopulationCommand(ClientManager* clientManager) : _clientManager(clientManager) {}

void PopulationCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
    (void)parsedMsg; // To avoid unused parameter warning
    
    std::string response = "Server Population:\n";
    send(client_fd, response.c_str(), response.length(), 0);
    _clientManager->printClientList(client_fd);
    
    // Also print to server console
    std::cout << "Population command executed by client " << client_fd << std::endl;
}
