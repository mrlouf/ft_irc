/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RegisteredClient.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmunoz-g <hmunoz-g@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:15:00 by hmunoz-g          #+#    #+#             */
/*   Updated: 2025/03/17 15:32:11 by hmunoz-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REGISTEREDCLIENT_HPP
# define REGISTEREDCLIENT_HPP

# include <string>

class RegisteredClient {
private:
    int _fd;
    std::string _nickname;
    std::string _username;
    bool _online;

public:
    RegisteredClient();
    RegisteredClient(int fd, const std::string &nickname, const std::string &username);
    ~RegisteredClient();

    // Getters
    int getFd() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;
    bool isOnline() const;

    // Setters
    void setFd(int fd);
    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setOnline(bool status);
};

#endif