/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:24:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 12:10:08 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CommandManager.hpp"
#include "../../includes/ChannelManager.hpp"

// Constructor and Destructor

CommandManager::CommandManager(ClientManager *clientManager, ChannelManager *channelManager, ServerManager *serverManager) {
    _commands["POPULATION"] = new PopulationCommand(clientManager);
	_commands["CHANNELLIST"] = new ChannelListCommand(channelManager);
    _commands["QUIT"] = new QuitCommand(serverManager);
	_commands["JOIN"] = new JoinCommand(channelManager, clientManager);
}

CommandManager::~CommandManager() {
	for (std::map<std::string, ICommand*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
		delete it->second;
	}
}

ParsedMessage CommandManager::parseInput(const std::string &input){
	ParsedMessage parsedMsg;
	std::istringstream stream(input);
	std::string token;

	// Check for prefix
	if (!input.empty() && input[0] == ':') {
		stream >> parsedMsg.prefix;
		parsedMsg.prefix = parsedMsg.prefix.substr(1);
	}

	// Read command
	if (stream >> parsedMsg.command) {
		while (stream >> token) {
			if (token[0] == ':') {
				std::string rest;
				std::getline(stream, rest);
				parsedMsg.params.push_back(token.substr(1) + rest);
				break;  // Stop reading more params
			}
			parsedMsg.params.push_back(token);
		}
	}

	return parsedMsg;
}

void CommandManager::executeCommand(int client_fd, const std::string& input) {
    ParsedMessage parsedMsg = parseInput(input); // Use our parser

    if (parsedMsg.command.empty()) {
        return; // Ignore empty input
    }

    // Convert command to uppercase (IRC commands are case-insensitive)
    std::transform(parsedMsg.command.begin(), parsedMsg.command.end(), 
                   parsedMsg.command.begin(), ::toupper);

    // Check if command exists in our map
    if (_commands.find(parsedMsg.command) != _commands.end()) {
        _commands[parsedMsg.command]->executeCommand(client_fd, parsedMsg);
    } else {
        send(client_fd, "Unknown command\n", 16, 0);
    }
}
