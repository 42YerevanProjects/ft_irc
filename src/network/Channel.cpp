/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/26 20:34:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/26 20:42:09 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Channel.hpp"


/* Constructor and Destructor */

Channel::Channel(const std::string& name, const std::string& key, Client* admin)
    : _name(name), _admin(admin), _k(key), _l(0), _n(false)
{

}

Channel::~Channel() {}


/* Getters */

std::string                 Channel::get_name() const { return _name; }
Client*                     Channel::get_admin() const { return _admin; }

std::string                 Channel::get_key() const { return _k; }
size_t                      Channel::get_limit() const { return _l; }
bool                        Channel::ext_msg() const { return _n; }

size_t                      Channel::get_size()const { return _clients.size(); }

std::vector<std::string>    Channel::get_nicknames()
{
    std::vector<std::string> nicknames;

    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        Client* client = *it_b;

        std::string nick = (client == _admin ? "@" : "") + client->get_nickname();
        nicknames.push_back(nick);

        it_b++;
    }

    return nicknames;
}


/* Setters */

void                        Channel::set_key(std::string key) { _k = key; }
void                        Channel::set_limit(size_t limit) { _l = limit; }
void                        Channel::set_ext_msg(bool flag) { _n = flag; }


/* Channel Actions */

void                        Channel::send_message(Client *client, const std::string& message)
{
    std::string buffer = message + "\r\n";
    int fd = client->get_fd();
    int n = buffer.length();

    if (send(fd, buffer.c_str(), n, 0) < 0)
        throw std::runtime_error("Error while sending a message to a client!");
}

void                        Channel::broadcast(const std::string& message)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        this->send_message(*it_b, message);
        it_b++;
    }
}

void                        Channel::broadcast(const std::string& message, Client* exclude)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (*it_b == exclude)
        {
            it_b++;
            continue;
        }

        this->send_message(*it_b, message);
        it_b++;
    }
}

void                        Channel::add_client(Client* client)
{
    _clients.push_back(client);
}

void                        Channel::remove_client(Client* client)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (*it_b == client)
            _clients.erase(it_b);
        
        it_b++;
    }

    if (client == _admin)
    {
        _admin = *(_clients.begin());

        std::string message = client->get_nickname() + " is now the admin of the channel " + _name;
        log(message);
    }
}