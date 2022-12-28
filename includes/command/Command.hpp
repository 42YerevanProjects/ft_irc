/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 18:08:15 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/28 18:24:08 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_COMMAND_HPP
#define IRC_COMMAND_HPP

#include <numeric>
#include <string>
#include <regex>

class Command;

#include "network/Server.hpp"

//==========================
//  THE BASE COMMAND CLASS
//==========================  

class Command
{
    protected:

        Server* _srv;
        bool    _auth;

        Command();
        Command(const Command& src);

    public:

        explicit Command(Server* srv, bool auth = true);
        virtual ~Command();

        bool auth_required() const;

        virtual void execute(Client* client, std::vector<std::string> args) = 0;
};


#endif
