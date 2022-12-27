/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/24 20:58:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/24 21:12:09 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"

/* Constructor and Destructor */

Server::Server(const std::string &port, const std::string &pass) 
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _running = 1;
    _sock = create_socket();
}

Server::~Server() {}


/* Initialize and Listen */

void            Server::start()
{

}


/* Getters */

std::string     Server::get_password() const 
{
    return _pass;
}

Client*         Server::get_client(const std::string& nickname)
{
    client_iterator it_b = _clients.begin();
    client_iterator it_e = _clients.end();

    while (it_b != it_e)
    {
        if (!nickname.compare(it_b->second->get_nickname()))
            return it_b->second;

        it_b++;
    }

    return NULL;
}

Channel*        Server::get_channel(const std::string& name)
{
    channel_iterator it_b = _channels.begin();
    channel_iterator it_e = _channels.begin();

    while (it_b != it_e)
    {
        if (!name.compare((*it_b)->get_name()))
            return (*it_b);

        it_b++;
    }

    return NULL;
}


/* Handle Clients*/

void            Server::on_client_connect()
{
    // accept a connection

    int         fd;
    sockaddr_in addr = {};
    socklen_t   size = sizeof(addr);

    fd = accept(_sock, (sockaddr *) &addr, &size);
    if (fd < 0)
        throw std::runtime_error("Error while accepting a new client!");

    // including the client fd in the poll

    pollfd  pfd = {fd, POLLIN, 0};
    _pfds.push_back(pfd);

    // getting hostname from the client address

    char hostname[NI_MAXHOST];
    int res = getnameinfo((struct sockaddr *) &addr, sizeof(addr), hostname, NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
    if (res != 0)
        throw std::runtime_error("Error while getting a hostname on a new client!");

    // creating and saving a new client

    Client* client = new Client(fd, ntohs(addr.sin_port), hostname);
    _clients.insert(std::make_pair(fd, client));

    // logging connect message

    char message[1000];
    sprintf(message, "%s:%d has connected.", client->get_hostname().c_str(), client->get_port());
    log(message);
}

void            Server::on_client_disconnect(int fd)
{
    try
    {
        // Finding the client and removing

        Client* client = _clients.at(fd);

        client->leave();
        _clients->erase(fd);

        // Removing the client fd from the poll

        pfd_iterator it_b = _pfds.begin();
        pfd_iterator it_e = _pfds.end();

        while (it_b != it_e)
        {
            if (it_b->fd == fd)
            {
                _pfds.erase(it_b);
                close(fd);
                break;
            }
        }

        // Release memory

        delete client;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error while disconnecting! " << e.what() << std::endl;
    }
}

/* Create Socket */

int             Server::create_socket()
{
    // opening a socket

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0)
        throw std::runtime_error("Error while opening a socket!");

    // forcefully attacing socket to the port by making it reusable

    int optval = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
        throw std::runtime_error("Error while setting socket options!");

    // making the socket NON-BLOCKING

    if (fcntl(sock_fd, F_SETFL, O_NONBLOCK))
        throw std::runtime_error("Error while setting socket to NON-BLOCKING!");

    // gathering neccessary data for binding
    
    struct sockaddr_in  serv_addr = {};

    bzero((char*) &serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(_port.c_str()));

    // bind socket to an IP address on selected port
    
    if (bind(sock_fd, (sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("Error while binding a socket!");

    // listening for requests

    if (listen(sock_fd, MAX_CONNECTIONS) < 0)
        throw std::runtime_error("Error while listening on a socket!");

    return sock_fd;
}
