#include "../../includes/commands/PrivmsgCommand.hpp"
#include "../../includes/ChannelManager.hpp"
#include "../../includes/ClientManager.hpp"

// Constructor
PrivmsgCommand::PrivmsgCommand(ChannelManager* channelManager, ClientManager* clientManager) 
    : _channelManager(channelManager), _clientManager(clientManager) {}

// Destructor
PrivmsgCommand::~PrivmsgCommand() {}

// Execute the PRIVMSG command
void PrivmsgCommand::executeCommand(int client_fd, const ParsedMessage& parsedMsg) {
    //!Erase this before turning in
    std::cout << "arrived at privmsg command" << std::endl;

    if (parsedMsg.params.size() < 2) {
        std::string errorMsg = "ERROR: PRIVMSG requires a channel and message\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    std::string channelName = parsedMsg.params[0];
    std::string message = parsedMsg.params[1];

    // Find the client by FD
    RegisteredClient* client = _clientManager->getClientFromFd(client_fd);
    if (!client) {
        std::string errorMsg = "ERROR: You must be a registered client to send messages\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    // Find the channel
    Channel* channel = _channelManager->getChannel(channelName);
    if (!channel) {
        std::string errorMsg = "ERROR: No such channel: " + channelName + "\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    // Ensure client is a member of the channel
    if (!channel->isMember(client)) {
        std::string errorMsg = "ERROR: You must join the channel before sending messages\r\n";
        send(client_fd, errorMsg.c_str(), errorMsg.length(), 0);
        return;
    }

    // Broadcast the message to other members
    broadcastMessage(message, channel, client);
}

// Helper method to broadcast the message to all channel members
void PrivmsgCommand::broadcastMessage(const std::string& message, Channel* channel, RegisteredClient* sender) {
    std::string formattedMessage = ":" + sender->getNickname() + " PRIVMSG " + channel->getName() + " :" + message + "\r\n";

    // Send the message to all channel members except the sender
    std::vector<RegisteredClient*>::const_iterator it;
    for (it = channel->getMembers().begin(); it != channel->getMembers().end(); ++it) {
        RegisteredClient* member = *it;
        if (member != sender) {
            send(member->getFd(), formattedMessage.c_str(), formattedMessage.length(), 0);
        }
    }
}