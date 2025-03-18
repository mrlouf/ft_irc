/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:20:26 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 15:17:11 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ClientManager.hpp"

// Constructor and destructor
ClientManager::ClientManager(const std::string &password): _password(password) {}

ClientManager::~ClientManager() {}

//Getters and setters
std::map<std::string, RegisteredClient> &ClientManager::getRegisteredClients(void){
    return (_registeredClients);
}

std::map<int, RegisteredClient> &ClientManager::getRegisteredFds(void){
    return (_registeredFds);
}

RegisteredClient *ClientManager::getClientFromNickname(std::string &nickname) {
    if (_registeredClients.find(nickname) != _registeredClients.end()) {
        return &_registeredClients[nickname];
    }
    return NULL;
}

RegisteredClient *ClientManager::getClientFromFd(int fd) {
    if (_registeredFds.find(fd) != _registeredFds.end()) {
        return &_registeredFds[fd];
    }
    return NULL;
}

// Methods
bool ClientManager::authenticateClient(const std::string &input) {
    if (input == _password) {
        std::cout << "Client authenticated." << std::endl;
        return true;
    }
    return false;
}

bool ClientManager::isNicknameRegistered(const std::string& nickname) const {
    return _registeredClients.find(nickname) != _registeredClients.end();
}

bool ClientManager::isRegistered(std::string &nickname, int client_fd) {
    if (_registeredClients.find(nickname) != _registeredClients.end()) {
        std::cerr << "Client " << nickname << " is already registered." << std::endl;
        if (_registeredClients[nickname].getFd() != client_fd)
            _registeredClients[nickname].setFd(client_fd);
        return true;
    }
    return false;
}

bool ClientManager::registerClient(const std::string &nickname, const std::string &username, int client_fd) {
    _registeredClients[nickname] = RegisteredClient(client_fd, nickname, username);
    _registeredFds[client_fd] = RegisteredClient(client_fd, nickname, username);
    std::cout << "Client " << client_fd << " registered with nickname: " << nickname << " and username: " << username << std::endl;
    return true;
}

void ClientManager::setClientOffline(int fd) {
    // Iterate through the map to find the client with the matching fd
    for (std::map<std::string, RegisteredClient>::iterator it = _registeredClients.begin(); it != _registeredClients.end(); ++it) {
        if (it->second.getFd() == fd) {
            std::cout << "Setting client offline: " << it->first << " (fd: " << fd << ")" << std::endl;
            it->second.setFd(-1); // Set fd to -1 to indicate offline status
            it->second.setOnline(false);
            break ;
        }
    }

    //Clear the fd from the register map
    std::map<int, RegisteredClient>::iterator it = _registeredFds.find(fd);
    if (it != _registeredFds.end()) {
        _registeredFds.erase(it);
    }
}

bool ClientManager::reconnectClient(const std::string& nickname, int new_fd) {
    if (_registeredClients.find(nickname) != _registeredClients.end()) {
        // Check if this client is already online with a different fd
        if (_registeredClients[nickname].isOnline() && _registeredClients[nickname].getFd() != -1) {
            std::cerr << "Client " << nickname << " is already connected with a different fd." << std::endl;
            return false;
        }
        
        // Update the fd and set online status
        _registeredClients[nickname].setFd(new_fd);
        _registeredClients[nickname].setOnline(true);

        //Reassign fds if necessary
        if (_registeredFds.find(new_fd) != _registeredFds.end()) {
            _registeredFds[new_fd] = _registeredClients[nickname];
        } else {
            _registeredFds[new_fd] = _registeredClients[nickname];
        }

        std::cout << "Client " << nickname << " reconnected with fd: " << new_fd << std::endl;
        return true;
    }
    return false;
}

void ClientManager::unregisterClient(const std::string &nickname, int client_fd) {
    (void)client_fd;
    std::map<std::string, RegisteredClient>::iterator it = _registeredClients.find(nickname);
    if (it != _registeredClients.end()) {
        int fd = it->second.getFd();
        //TODO: remove client from channels
        
        // Remove from the FD-based map first
        std::map<int, RegisteredClient>::iterator fdIt = _registeredFds.find(fd);
        if (fdIt != _registeredFds.end()) {
            _registeredFds.erase(fdIt);
        }

        // Remove from the nickname-based map
        _registeredClients.erase(it);
    }
}

// Debugging
void ClientManager::printClientList() const {
    for (std::map<std::string, RegisteredClient>::const_iterator it = _registeredClients.begin(); it != _registeredClients.end(); ++it) {
        std::string status = it->second.isOnline() ? "online" : "offline";
        std::cout << it->second.getNickname() << "-" 
                  << it->second.getUsername() << "-" 
                  << it->second.getFd() << "-" 
                  << status << std::endl;
    }
}

/*std::string ClientManager::getClientListAsString() const {
    std::string result;
    
    // Check if there are any clients
    if (_registeredClients.empty()) {
        return "No clients connected.";
    }
    
    // Iterate through the registered clients
    for (std::map<int, RegisteredClient*>::const_iterator it = _registeredClients.begin(); it != _registeredClients.end(); ++it) {
        result += "Client FD: " + std::to_string(it->first);
        
        // Add any additional client information you want to display
        // For example, if your Client class has a getNickname() method:
        if (it->second->isRegistered()) {
            result += " - Nickname: " + it->second->getNickname();
        } else {
            result += " - Not registered";
        }
        
        result += "\n";
    }
    
    return result;
}*/