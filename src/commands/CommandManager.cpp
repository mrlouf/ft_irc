/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:24:23 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 12:31:08 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/CommandManager.hpp"

// Constructor and Destructor

CommandManager::CommandManager(ClientManager* clientManager, ServerManager* serverManager) {
    _commands["POPULATION"] = new PopulationCommand(clientManager);
    _commands["QUIT"] = new QuitCommand(serverManager);
}

CommandManager::~CommandManager() {
	for (std::map<std::string, ICommand*>::iterator it = _commands.begin(); it != _commands.end(); ++it) {
		delete it->second;
	}
}

void CommandManager::executeCommand(int client_fd, const std::string& input) {
	std::istringstream iss(input);
	std::vector<std::string> args;
	std::string token;
	while (iss >> token)
		args.push_back(token);

	if (args.empty())
		return;

	std::string commandName = args[0];

	std::transform(commandName.begin(), commandName.end(), commandName.begin(), (int(*)(int))std::toupper);

	if (_commands.find(commandName) != _commands.end()) {
		_commands[commandName]->executeCommand(client_fd, args);
	} else {
		send(client_fd, "Unknown command\n", 16, 0);
	}
}
