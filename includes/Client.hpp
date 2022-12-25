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

#include "Channel.hpp"
#include "response.hpp"


enum ClientState
{
    HANDSHAKE,
    LOGIN,
    PLAY,
    DISCONNECTED
};


class Client 
{
    typedef std::vector<pollfd>::iterator   pfd_iterator;

    private:
        
        int             _fd;
        int             _port;

        std::string     _nickname;
        std::string     _username;
        std::string     _realname;
        std::string     _hostname;

        ClientState     _state;

        Client();
        Client(const Client &src);

    public:

        //TODO: Implement member functions
};

#endif
