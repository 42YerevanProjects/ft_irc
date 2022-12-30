/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
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


//=======================
//  THE COMMAND CLASSES
//=======================

class Notice : public Command
{
    public:

        Notice(Server* srv);
        ~Notice();

        void    execute(Client* client, std::vector<std::string> args);
};

class PrivMsg : public Command
{
    public:

        PrivMsg(Server* srv);
        ~PrivMsg();

        void    execute(Client* client, std::vector<std::string> args);
};

class Part : public Command
{
    public:

        Part(Server* srv);
        ~Part();

        void    execute(Client* client, std::vector<std::string> args);
};

class Quit : public Command
{
    public:

        Quit(Server* srv, bool auth);
        ~Quit();

        void    execute(Client* client, std::vector<std::string> args);
};

class Join : public Command
{
    public:

        Join(Server* srv);
        ~Join();

        void    execute(Client* client, std::vector<std::string> args);
};

class User : public Command
{
    public:

        User(Server* srvi, bool auth);
        ~User();

        void    execute(Client* client, std::vector<std::string> args);
};

class Nick : public Command
{
    public:

        Nick(Server* srv, bool auth);
        ~Nick();

        void    execute(Client* client, std::vector<std::string> args);
};

class Pass : public Command
{
    public:

        Pass(Server* srv, bool auth);
        ~Pass();

        void    execute(Client* client, std::vector<std::string> args);
};

class Kick : public Command
{
    public:

        Kick(Server* srv);
        ~Kick();

        void    execute(Client* client, std::vector<std::string> args);
};

class Ping : public Command
{
    public:

        Ping(Server* srv);
        ~Ping();

        void    execute(Client* client, std::vector<std::string> args);
};

class Pong : public Command
{
    public:

        Pong(Server* srv);
        ~Pong();

        void    execute(Client* client, std::vector<std::string> args);
};

class Mode : public Command
{
    public:

        Mode(Server* srv);
        ~Mode();

        void    execute(Client* client, std::vector<std::string> args);
};

#endif
