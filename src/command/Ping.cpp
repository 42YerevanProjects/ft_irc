/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 04:04:09 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 04:15:29 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Ping::Ping(Server* srv) : Command(srv) {}

Ping::~Ping() {}

// syntax: PING <server1> [<server2>]

void    Ping::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PING"));
        return;
    }

    client->write(RPL_PING(client->get_prefix(), args.at(0)));
}
