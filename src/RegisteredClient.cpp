/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegisteredClient.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:17:54 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/17 15:32:26 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/RegisteredClient.hpp"

RegisteredClient::RegisteredClient() : _fd(-1), _nickname(""), _username(""), _online(false) {}

RegisteredClient::RegisteredClient(int fd, const std::string &nickname, const std::string &username) 
    : _fd(fd), _nickname(nickname), _username(username), _online(true) {}

RegisteredClient::~RegisteredClient() {}

// Getters
int RegisteredClient::getFd() const {
    return _fd;
}

const std::string &RegisteredClient::getNickname() const {
    return _nickname;
}

const std::string &RegisteredClient::getUsername() const {
    return _username;
}

bool RegisteredClient::isOnline() const {
    return _online;
}

// Setters
void RegisteredClient::setFd(int fd) {
    _fd = fd;
}

void RegisteredClient::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

void RegisteredClient::setUsername(const std::string &username) {
    _username = username;
}

void RegisteredClient::setOnline(bool status) {
    _online = status;
}