/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:07:49 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/19 17:23:21 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
# include <map>
# include <algorithm>
# include <arpa/inet.h>
# include <netinet/in.h>

# include "RegisteredClient.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::vector<RegisteredClient*> _members;
		std::vector<RegisteredClient*> _operators;

	public:
		// Constructor and Destructor
		Channel();
		Channel(const std::string &name);
		~Channel();

		// Getters and Setters
		const std::string &getName() const;
		const std::string &getTopic() const;
		const std::vector<RegisteredClient*> &getMembers() const;
		const std::vector<RegisteredClient*> &getOperators() const;

		void setTopic(const std::string &topic);
		void setName(const std::string &name);

		// Methods
		bool isMember(RegisteredClient *client);
		bool addMember(RegisteredClient *client);
		bool addOperator(RegisteredClient *oper);
		bool removeMember(RegisteredClient *client);
		bool RemoveOperator(RegisteredClient *oper);
		void broadcastMessage(const std::string& message, RegisteredClient* sender);

};

#endif