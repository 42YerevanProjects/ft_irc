/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 19:06:42 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/25 19:15:51 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Client.hpp"

/* Constructor and Destructor */

Client::Client(int, fd, int, port, const std::string &hostname)
    : _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE) {}


Client::~Client() {}


/* Getters */

int             Client::get_fd() const { return _fd; }
int             Client::get_port const { return _port; }

std::string     Client::get_nickname() const { return _nickname; }
std::string     Client::get_username() const { return _username; }
std::string     Client::get_realname() const { return _realname; }
std::string     Client::get_hostname() const { return _hostname; }

std::string     Client::get_prefix() const 
{
    std::string username = _username.empty() ? "" : "!" + _username;
    std::string hostname = _hostname.empty() ? "" : "@" + _hostname;

    return _nickname + username + hostname;
}


/* Setters */

void            Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
void            Client::set_username(const std::string &username) { _username = username; }
void            Client::set_realname(const std::string &realname) { _realname = realname; }
void            Client::set_state(ClientState state) { _state = state; }

