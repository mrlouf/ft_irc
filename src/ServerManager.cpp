/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 17:08:39 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 18:35:36 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerManager.hpp"

// Message macros
const std::string ServerManager::REGISTERERR = "Error registering user";
const std::string ServerManager::REGISTERSCCS = "User successfully registered with nickname ";
const std::string ServerManager::WELCOMEMSG = "Welcome to ft_irc!";
const std::string ServerManager::RECONECTSCCS = "Welcome back to ft_irc";

//Constructor and destructor
ServerManager::ServerManager(int port, const std::string &password) 
    : _socketManager(new SocketManager(port)), 
      _clientManager(new ClientManager(password)),
      _channelManager(new ChannelManager(_clientManager)), 
      _commandManager(new CommandManager(_clientManager, _channelManager, this)) {
}

ServerManager::~ServerManager() {
    delete _socketManager;
    delete _clientManager;
    delete _commandManager;
}

// Getters
ClientManager *ServerManager::getClientManager(){
    return (_clientManager);
}

// Methods
void ServerManager::run() {
    // Add the listening socket to the poll list
    struct pollfd server_poll_fd;
    server_poll_fd.fd = _socketManager->getServerFd();
    server_poll_fd.events = POLLIN;
    _fds.push_back(server_poll_fd);

    while (true) {
        // Poll for activity on any socket
        int poll_count = poll(&_fds[0], static_cast<unsigned int>(_fds.size()), -1);
        if (poll_count < 0) {
            std::cerr << "Poll error." << std::endl;
            break;
        }

        for (size_t i = 0; i < _fds.size(); ++i) {
            // New client connection
            if (_fds[i].fd == _socketManager->getServerFd() && (_fds[i].revents & POLLIN)) {
                int client_fd = _socketManager->acceptConnection();
                if (client_fd != -1) {
                    std::cout << "Client connected (fd:" << client_fd << ")" << std::endl;

                    struct pollfd client_poll_fd;
                    client_poll_fd.fd = client_fd;
                    client_poll_fd.events = POLLIN | POLLHUP;
                    _fds.push_back(client_poll_fd);
                }
            } else if (_fds[i].revents & POLLIN) {
                std::string input;
                if (!readFromClient(_fds[i].fd, input)) {
                    std::cout << "Client disconnected (fd: " << _fds[i].fd << ")" << std::endl;
                    _clientManager->setClientOffline(_fds[i].fd);
                    _clientManager->clearPendingRegistration(_fds[i].fd);
                    close(_fds[i].fd);
                    _fds.erase(_fds.begin() + i);
                    --i;
                } else {
                    std::cout << "INPUT:" << input << std::endl;
                    _commandManager->executeCommand(_fds[i].fd, input);
                }
            }
        }
    }
}

bool ServerManager::readFromClient(int client_fd, std::string &input) {
    char buffer[1024];
    ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (bytes_read <= 0) {
        return false;
    }
    
    buffer[bytes_read] = '\0';
    input = buffer;
    
    if (input.length() >= 2 && input[input.length() - 2] == '\r' && input[input.length() - 1] == '\n') {
        input = input.substr(0, input.length() - 2);
    }
    else if (input.length() >= 1 && input[input.length() - 1] == '\n') {
        input = input.substr(0, input.length() - 1);
    }
    
    return true;
}

void ServerManager::disconnectClient(const std::string &nickname, int client_fd) {
	// Get the client's file descriptor from the registered client map
	const std::map<std::string, RegisteredClient>& clientList = _clientManager->getRegisteredClients();
	std::map<std::string, RegisteredClient>::const_iterator it = clientList.find(nickname);

	if (it != clientList.end()) {
		// Check if the fd matches the one provided
		if (it->second.getFd() == client_fd) {
			// Let ClientManager handle the removal from _registeredClients
			_clientManager->unregisterClient(nickname, client_fd);
			
			// Close the socket
			close(client_fd);
			
			// Remove from the poll list
			for (size_t i = 0; i < _fds.size(); ++i) {
				if (_fds[i].fd == client_fd) {
					_fds.erase(_fds.begin() + i);
					break;
				}
			}

			std::cout << "Client " << nickname << " disconnected." << std::endl;
		} else {
			std::cerr << "Provided fd does not match the client's registered fd." << std::endl;
		}
	} else {
		std::cerr << "Client " << nickname << " not found in the registered clients list." << std::endl;
	}
}