/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WhoCommand.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:17:34 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/20 12:37:42 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/commands/WhoCommand.hpp"

# include "../../includes/ClientManager.hpp"
# include "../../includes/ServerManager.hpp"
# include "../../includes/ChannelManager.hpp"

WhoCommand::WhoCommand(ServerManager *serverManager, ChannelManager *channelManager)
    : _serverManager(serverManager), _channelManager(channelManager) {}

void WhoCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
    if (parsedMsg.params.empty()) {
        std::string errorMsg = ":server 461 Who :Not enough parameters\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string channelName = parsedMsg.params[0];

    Channel* channel = _channelManager->getChannel(channelName);
    if (!channel) {
        std::string errorMsg = ":server 403 " + channelName + " :No such channel\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    sendWhoReply(client_fd, channel);
}

void WhoCommand::sendWhoReply(int client_fd, Channel* channel) {
	for (size_t i = 0; i < channel->getMembers().size(); ++i) {
		RegisteredClient* member = channel->getMembers()[i];
		std::string modes = (channel->isOperator(member) ? "@" : "");
		std::string realName = member->getUsername();

		std::stringstream timeStream;
		timeStream << time(NULL);
		std::string timeStr = timeStream.str();

		std::string WhoMessage = ":" + _serverManager->getServerName() + " 352 " +
								member->getNickname() + " " + channel->getName() + " " +
								member->getNickname() + " " + member->getHost() + " " +
								_serverManager->getServerName() + " " + timeStr + " " +
								modes + " :" + realName + "\r\n";
		send(client_fd, WhoMessage.c_str(), WhoMessage.length(), 0);
	}

	std::string endMessage = ":" + _serverManager->getServerName() + " 366 " +
							channel->getName() + " :End of /Who list.\r\n";
	send(client_fd, endMessage.c_str(), endMessage.length(), 0);
}