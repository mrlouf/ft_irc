/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:24:49 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 16:27:47 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDMANAGER_HPP
# define COMMANDMANAGER_HPP

# include <string>
# include <map>
# include <vector>
# include <sstream>
# include <algorithm>
# include <cctype>
# include <unistd.h>
# include <sys/types.h>
# include <sys/socket.h>

# include "PopulationCommand.hpp"
# include "QuitCommand.hpp"

class ServerManager;

class CommandManager {
	private:
		std::map<std::string, ICommand*> _commands;

	public:
		// Constructor and Destructor
		CommandManager(ClientManager* clientManager, ServerManager* serverManager);
		~CommandManager();

		// Methods
		void executeCommand(int client_fd, const std::string &input);
		ParsedMessage parseInput(const std::string &input);
};

#endif