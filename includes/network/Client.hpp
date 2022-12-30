/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 15:34:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/25 16:36:09 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_CLIENT_HPP
#define IRC_CLIENT_HPP

#include <sys/socket.h>
#include <sys/poll.h>

#include <string>
#include <vector>

class Client;

#include "Channel.hpp"
#include "response.hpp"


enum ClientState
{
    HANDSHAKE,
    LOGIN,
    REGISTERED,
    DISCONNECTED
};


class Client 
{
    private:
        
        int             _fd;
        int             _port;

        std::string     _nickname;
        std::string     _username;
        std::string     _realname;
        std::string     _hostname;

        ClientState     _state;
        Channel*        _channel;

        Client();
        Client(const Client &src);

    public:

        /* Costructor and Destructor */
        
        Client(int fd, int port, const std::string &hostname);
        ~Client();


        /* Getters */

        int             get_fd() const;
        int             get_port() const;

        std::string     get_nickname() const;
        std::string     get_username() const;
        std::string     get_realname() const;
        std::string     get_hostname() const;
        std::string     get_prefix() const;

        Channel*        get_channel() const;

    
        /* Setters */

        void            set_nickname(const std::string &nickname);
        void            set_username(const std::string &username);
        void            set_realname(const std::string &realname);
        void            set_state(ClientState state);
        void            set_channel(Channel *channel);


        /* Check state */

        bool            is_registered() const;


        /* Send/Recieve Actions */

        void            write(const std::string& message) const;
        void            reply(const std::string& reply);

        void            welcome();


        /* Client Actions */

        void            join(Channel *channel);
        void            leave();
};

#endif
