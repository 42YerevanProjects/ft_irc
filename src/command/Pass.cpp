/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 03:35:09 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 03:42:29 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Command.hpp"

Pass::Pass(Server* srv) : Command(srv) {}

Pass::~Pass() {}

// syntax: PASS <password>

void    Pass::execute(Client* client, std::vetor<std::string> args)
{
    if (args.empty())
    {
        client->reply(ERR_NEEDMOREPARAMS(client->get_nickname(), "PASS"));
        return;
    }

    if (client->is_registered())
    {
        client->reply(ERR_ALREADYREGISTERED(client->get_nickname()));
        return;
    }

    if (_srv->get_pass() != args[0].substr(args[0][0] == ':' ? 1 : 0))
    {
        client->reply(ERR_PASSWDMISMATCH(client->get_nickname()));
        return;
    }

    client->set_state(LOGIN);
}