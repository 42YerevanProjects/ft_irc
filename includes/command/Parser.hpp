/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 13:52:51 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/30 13:58:53 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_PARSER_HPP
#define IRC_PARSER_HPP

#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <map>

class Parser;

#include "Command.hpp"

class Parser
{
    private:

        Server*                             _srv;
        std::map<std::string, Command *>    _commands;

        std::string     trim(const std::string& str);

    public:

        Parser(Server* srv);
        ~Parser();

        void    invoke(Client* client, const std::string& message);
};

#endif
