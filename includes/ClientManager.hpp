/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 16:16:37 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 14:47:56 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTMANAGER_HPP
# define CLIENTMANAGER_HPP

# include <map>
# include <string>
# include <iostream>
# include "RegisteredClient.hpp"

class ClientManager {
private:
    std::string _password;
    std::map<std::string, RegisteredClient> _registeredClients;
    std::map<int, RegisteredClient> _registeredFds;

    // Coplien stuff
    ClientManager();
    ClientManager(const ClientManager &other);
    ClientManager &operator=(const ClientManager &other);

public:
    // Constructor and destructor
    ClientManager(const std::string &password);
    ~ClientManager();

    // Getters and setters
    std::map<std::string, RegisteredClient> &getRegisteredClients(void);
    std::map<int, RegisteredClient> &getRegisteredFds(void);
    RegisteredClient *getClientFromNickname(std::string &nickname);
    RegisteredClient *getClientFromFd(int fd);

    // Methods
    bool authenticateClient(const std::string &input);
    bool isNicknameRegistered(const std::string& nickname) const;
    bool isRegistered(std::string &nickname, int client_fd);
    bool registerClient(const std::string &nickname, const std::string &username, int client_fd);
    void setClientOffline(int fd);
    bool reconnectClient(const std::string &nickname, int new_fd);
    void unregisterClient(const std::string &nickname, int client_fd);
    void printClientList() const;
    std::string getClientListAsString() const;
};

#endif