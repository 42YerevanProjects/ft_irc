/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 14:14:15 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/30 14:30:19 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command/Parser.hpp"

Parser::Parser(Server* srv) : _srv(srv)
{
    _commands["PASS"] = new Pass(_srv, false);
    _commands["NICK"] = new Nick(_srv, false);
    _commands["USER"] = new User(_srv, false);
    _commands["QUIT"] = new Quit(_srv, false);

    _commands["PING"] = new Ping(_srv);
    _commands["PONG"] = new Pong(_srv);
    _commands["JOIN"] = new Join(_srv);
    _commands["PART"] = new Part(_srv);
    _commands["KICK"] = new Kick(_srv);
    _commands["MODE"] = new Mode(_srv);

	_commands["PRIVMSG"] = new PrivMsg(_srv);
	_commands["NOTICE"] = new Notice(_srv);
}

Parser::~Parser ()
{
    std::map<std::string, Command *>::iterator it = _commands.begin();
    std::map<std::string, Command *>::iterator end = _commands.end();

    while (it != end)
    {
        delete it->second;
        it++;
    }
}


std::string     Parser::trim(const std::string& str)
{
    std::string WHITESPACE = " \n\r\t\f\v";
    std::string result = "";

    size_t  start = str.find_first_not_of(WHITESPACE);
    if (start != std::string::npos)
        result = str.substr(start);

    size_t  end = result.find_last_not_of(WHITESPACE);
    if (end != std::string::npos)
        result = result.substr(0, end + 1);

    return result;
}

void            Parser::invoke(Client* client, const std::string& message)
{
    std::stringstream   ss(message);
    std::string         syntax;
    
    while (std::getline(ss, syntax))
    {
        syntax = trim(syntax);

        std::string name = syntax.substr(0, syntax.find(' '));

        try
        {
            std::vector<std::string>    args;
            std::stringstream           line(syntax.substr(name.length(), syntax.length()));
            std::string                 buf;

            Command *cmd = _commands.at(name);

            while (line >> buf)
                args.push_back(buf);


            if (!client->is_registered() && cmd->auth_required())
            {
                client->reply(ERR_NOTREGISTERED(client->get_nickname()));
                return;
            }

            cmd->execute(client, args);
        }
        catch (const std::exception& e)
        {
            client->reply(ERR_UNKNOWNCOMMAND(client->get_nickname(), name));
        }
    }
}
