/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/24 15:34:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/24 16:36:09 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_SERVER_HPP
#define IRC_SERVER_HPP

#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdexcept>
#include <cerrno>
#include <string>

#include <algorithm>
#include <vector>
#include <map>

class Server;

#include "Client.hpp"
#include "Channel.hpp"
#include "command/Parser.hpp"


#define MAX_CONNECTIONS 999

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

        Parser*                 _parser;

        Server();
        Server(const Server& src);

    public:

        /* Constructor and Destructor */

        Server(const std::string &port, const std::string &pass);
        ~Server();

        /* Initialize and Listen */

        void            start();

        /* Getters */

        std::string     get_password() const;
        Client*         get_client(const std::string &nickname);
        Channel*        get_channel(const std::string &name);

        /* Handle Clients */

        void            on_client_connect();
        void            on_client_disconnect(int fd);
        void            on_client_message(int fd);

        std::string     read_message(int fd);

        /* Create Channel */

        Channel*        create_channel(const std::string &name, const std::string &key, Client *client);

        /* Create Socket */

        int             create_socket();
};

#endif
