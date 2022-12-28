/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 22:56:10 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 23:07:45 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Part::Part(Server* srv) : Command(srv) {}

Part::~Part() {}

// syntax: PART <channels> [<message>]

void    Part::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty()) 
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PART"));
        return;
    }

    std::string name = args[0];
    Channel     *channel = _srv->get_channel(name);

    if (!channel || !client->get_channel() || client->get_channel()->get_name() != name)
    {
        client->reply(ERR_NOSUCHCHANNEL(client->get_nickname(), name));
        return;
    }

    client->leave();
}
