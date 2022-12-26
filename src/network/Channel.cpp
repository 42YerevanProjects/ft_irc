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

void                        Channel::broadcast(const std::string& message)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        (*it_b)->write(message);
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

        (*it_b)->write(message);
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

    client->set_channel(NULL);

    if (client == _admin)
    {
        _admin = *(_clients.begin());

        std::string message = client->get_nickname() + " is now the admin of the channel " + _name;
        log(message);
    }
}

void                        Channel::kick(Client* client, Client* target, const std::string& reason)
{
    this->broadcast(RPL_KICK(client->get_prefix(), _name, target->get_nickname(), reason));
    this->remove_client(target);

    std::string message = client->get_nickname() + " kicked " + target->get_nickname() + " from channel " + _name;
    log(message);
}
