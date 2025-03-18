/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 12:28:20 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/18 14:48:29 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUITCOMMAND_HPP
#define QUITCOMMAND_HPP

#include "ICommand.hpp"
#include <vector>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

class ServerManager;

class QuitCommand : public ICommand {
private:
    ServerManager *_serverManager;

public:
    QuitCommand(ServerManager* serverManager);
    void executeCommand(int client_fd, const std::vector<std::string>& args);
};

#endif