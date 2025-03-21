/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelManager.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:29:13 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 10:55:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

# include <map>
# include <vector>
# include <string>
# include <sstream> 
# include "objects/Channel.hpp"
# include <sys/types.h>
# include <sys/socket.h>

class ClientManager;

class ChannelManager {
	private:
    	std::map<std::string, Channel> _channels;
		ClientManager *_clientManager;

	public:
		// Constructor and Destructor
		ChannelManager(ClientManager *clientManager);
		~ChannelManager();

		// Getters and Setters
		const std::map<std::string, Channel> &getChannelList() const;
		Channel* getChannel(const std::string &name);
		std::vector<Channel*> getClientChannels(int client_fd);

		// Methods
		bool createChannel(const std::string &name);
		bool removeChannel(const std::string &name);
		void removeClientFromAllChannels(RegisteredClient *client);


		// Testing method
		void printChannelList(int client_fd);
};

#endif