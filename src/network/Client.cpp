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

Client::Client(int fd, int port, const std::string &hostname)
    : _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE), _channel(NULL) 
{

}


Client::~Client() {}


/* Getters */

int             Client::get_fd() const { return _fd; }
int             Client::get_port() const { return _port; }

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

Channel*        Client::get_channel() const { return _channel; }


/* Setters */

void            Client::set_nickname(const std::string &nickname) { _nickname = nickname; }
void            Client::set_username(const std::string &username) { _username = username; }
void            Client::set_realname(const std::string &realname) { _realname = realname; }
void            Client::set_state(ClientState state) { _state = state; }
void            Client::set_channel(Channel *channel) { _channel = channel; }


/* Check State */

bool            Client::is_registered() const { return _state == REGISTERED; }


/* Send/Recieve Actions */

void            Client::write(const std::string& message) const
{
    std::string buffer = message + "\r\n";
    if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
        throw std::runtime_error("Error while sending a message to a client!");
}

void            Client::reply(const std::string& reply)
{
    this->write(":" + get_prefix() + " " + reply);
}

void            Client::welcome()
{
    if (_state != LOGIN || _username.empty() || _realname.empty() || _nickname.empty())
		return;
    
    _state = REGISTERED;
    this->reply(RPL_WELCOME(_nickname));

    char message[100];
    sprintf(message, "%s:%d is now known as %s.", _hostname.c_str(), _port, _nickname.c_str());
    log(message);
}


/* Client Actions */

void            Client::join(Channel* channel)
{
    channel->add_client(this);
    _channel = channel;

    // Get users on the channel

    std::string users = "";
    std::vector<std::string> nicknames = channel->get_nicknames();
    std::vector<std::string>::iterator it_b = nicknames.begin();
    std::vector<std::string>::iterator it_e = nicknames.end();
    while (it_b != it_e)
    {
        users.append(*it_b + " ");
        it_b++;
    }

    // Send replies
    
    reply(RPL_NAMREPLY(_nickname, channel->get_name(), users));
    reply(RPL_ENDOFNAMES(_nickname, channel->get_name()));
    channel->broadcast(RPL_JOIN(get_prefix(), channel->get_name()));

    // log

    std::string message = _nickname + " has joined to the channel " + channel->get_name();
    log(message);
}

void            Client::leave()
{
    if (!_channel)
        return;

    const std::string name = _channel->get_name();

    _channel->broadcast(RPL_PART(get_prefix(), _channel->get_name()));
    _channel->remove_client(this);

    std::string message = _nickname + " has left the channel " + name;
    log(message);
}
