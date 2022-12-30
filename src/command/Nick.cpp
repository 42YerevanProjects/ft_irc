/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 03:24:09 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 03:34:29 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Nick::Nick(Server* srv, bool auth) : Command(srv, auth) {}

Nick::~Nick() {}

// syntax: NICK <nickname>

void    Nick::execute(Client* client, std::vector<std::string> args)
{
    if (args.empty() || args[0].empty()) 
    {
        client->reply(ERR_NONICKNAMEGIVEN(client->get_nickname()));
        return;
    }

    std::string nickname = args[0];

    if (_srv->get_client(nickname))
    {
        client->reply(ERR_NICKNAMEINUSE(client->get_nickname()));
        return;
    }

    client->set_nickname(nickname);
    client->welcome();
}
