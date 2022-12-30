/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 03:19:09 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 03:30:29 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

User::User(Server* srv, bool auth) : Command(srv, auth) {}

User::~User() {}

// stntax: USER <username> <hostname> <servername> <realname>

void    User::execute(Client* client, std::vector<std::string> args)
{
    if (client->is_registered())
    {
        client->reply(ERR_ALREADYREGISTERED(client->get_nickname()));
        return;
    }

    if (args.size() < 4) 
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "USER"));
        return;
    }

    client->set_username(args[0]);
    client->set_realname(args[3]);
    client->welcome();
}
