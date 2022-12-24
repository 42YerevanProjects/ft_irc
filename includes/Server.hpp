/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/24 15:34:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/18 16:36:09 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#include <unistd.h>
#include <fcntl.h>

#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include "Channel.hpp"
#include "Client.hpp"

class Server
{
    typedef std::vector<pollfd>::iterator       pfd_iterator;
    typedef std::vector<Channel *>::iterator    channel_iterator;
    typedef std::map<int, Client *>::iterator   client_iterator;

    private:
        
        int                     _running;
        int                     _sock;

        const std::string       _host;
        const std::string       _port;
        const std::string       _pass;

        std::vector<pollfd>     _pfds;
        std::vector<Channel *>  _channels;
        std::map<int, Client *> _clients;

};

#endif
