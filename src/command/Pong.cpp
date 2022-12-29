/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 04:18:09 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 04:24:08 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Pong::Pong(Server* srv) : Command(srv) {}

Pong::~Pong() {}

// syntax: PONG <server1> [<server2>]

void    Pong::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PONG"));
        return;
    }

    client->write(RPL_PING(client->get_prefix(), args.at(0)));
}
