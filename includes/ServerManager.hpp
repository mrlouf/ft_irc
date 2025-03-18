/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:07:50 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 14:44:26 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include <iostream>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include <arpa/inet.h>
# include <netinet/in.h>

# include "SocketManager.hpp"
# include "ClientManager.hpp"
#include "commands/CommandManager.hpp"

class ServerManager {
	private:
		//Coplien stuff
		ServerManager();
		ServerManager(const ServerManager &other);
		ServerManager &operator=(const ServerManager &other);
		
		std::vector<struct pollfd> _fds;
		SocketManager *_socketManager;
		ClientManager *_clientManager;
		CommandManager *_commandManager;

		//Message macros
		static const std::string REGISTERERR;
		static const std::string REGISTERSCCS;
		static const std::string RECONECTSCCS;
		static const std::string WELCOMEMSG;
		
	public:
		//Constructor and destructor
		ServerManager(int port, const std::string &password);
		~ServerManager();

		// Getters
		ClientManager *getClientManager();

		//Methods
		void run();
		void handleClientAttempt(int client_fd);
		bool requestPassword(int client_fd);
		std::string requestNickname(int client_fd);
		bool handleNewClientRegistration(int client_fd, const std::string& nickname);
		void welcomeBackClient(int client_fd, const std::string& nickname);
		bool readFromClient(int client_fd, std::string &output);
		void disconnectClient(const std::string &nickname, int client_fd);
};

#endif