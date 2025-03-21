/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 10:07:49 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/21 12:27:07 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>
# include <map>
# include <set>
# include <algorithm>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <climits>

# include "RegisteredClient.hpp"

class Channel {
	private:
		std::string _name;
		std::string _topic;
		std::string _password;
		size_t _userLimit;
		bool _passwordSet;
		bool _userLimitSet;
		bool _inviteOnlySet;
		bool _topicRestrictedSet;

		std::vector<RegisteredClient*> _members;
		std::vector<RegisteredClient*> _operators;
		std::set<char> _modes;

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
		bool isOperator(RegisteredClient *client);
		bool addMember(RegisteredClient *client);
		bool addOperator(RegisteredClient *oper);
		bool removeMember(RegisteredClient *client);
		bool removeOperator(RegisteredClient *oper);
		void setOperatorsToNoOps();
		void broadcastMessage(const std::string& message, RegisteredClient* sender);
		
		// MODE related methods
		void setMode(char mode, bool enable);
		void setPassword(const std::string &password);
		void setUserLimit(int limit);
		void setInviteOnly();
		void setTopicRestriction();
		bool isFull();
		bool isInviteOnly();
		bool isTopicRestricted();
		bool hasMode(char mode) const;
		void clearPassword();
		void clearUserLimit();
		void clearInviteOnly();
		void clearTopicRestriction();
		std::string getModeString() const;
		bool canJoin(const RegisteredClient *client, const std::string &password) const;
};

#endif