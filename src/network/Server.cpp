/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/24 20:58:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/29 19:17:27 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "network/Server.hpp"

/* Constructor and Destructor */

Server::Server(const std::string &port, const std::string &pass) 
    : _host("127.0.0.1"), _port(port), _pass(pass)
{
    _running = 1;
    _sock = create_socket();
    _parser = new Parser(this);
}

Server::~Server() 
{
    delete _parser;

    for (size_t i = 0; i < _channels.size(); i++)
        delete _clients[i];
}


/* Initialize and Listen */

void            Server::start()
{
    // add the server to the poll

    pollfd srv = {_sock, POLLIN, 0};
    _pfds.push_back(srv);

    log("Server is listening...");

    // running the main loop and waiting for connections

    while (_running)
    {
        if (poll(_pfds.begin().base(), _pfds.size(), -1) < 0)
            throw std::runtime_error("Error while polling from fd!");

        // one or more descriptors are readable => process them

        for (pfd_iterator it = _pfds.begin(); it != _pfds.end(); it++)
        {
            if (it->revents == 0)
                continue;

            if ((it->revents & POLLHUP) == POLLHUP)
            {
                this->on_client_disconnect(it->fd);
                break;
            }

            if ((it->revents & POLLIN) == POLLIN)
            {
                if (it->fd == _sock)
                {
                    this->on_client_connect();
                    break;
                }

                this->on_client_message(it->fd);
            }
        }
    }
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


/* Handle Clients */

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
        // finding the client and removing

        Client* client = _clients.at(fd);

        client->leave();

        // log about disconnecting 

        char message[1000];
		sprintf(message, "%s:%d has disconnected!", client->get_hostname().c_str(), client->get_port());
		log(message);

        _clients.erase(fd);

        // removing the client fd from the poll

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

            it_b++;
        }

        // release memory

        delete client;
    }
    catch (const std::exception &e)
    {
        std::cout << "Error while disconnecting! " << e.what() << std::endl;
    }
}

void            Server::on_client_message(int fd)
{
    try
    {
        Client*     client = _clients.at(fd);
        std::string message = this->read_message(fd);
        
        _parser->invoke(client, message);
    }
    catch (const std::exception& e) 
    {
        std::cout << "Error while handling the client message! " << e.what() << std::endl;
    }
}

std::string     Server::read_message(int fd)
{
    std::string message;
    
    char buffer[100];
    bzero(buffer, 100);

    while (!strstr(buffer, "\n"))
    {
        bzero(buffer, 100);

        if ((recv(fd, buffer, 100, 0) < 0) and (errno != EWOULDBLOCK))
            throw std::runtime_error("Error while reading buffer from a client!");

        message.append(buffer);
    }

    return message;
}


/* Create Channel */

Channel*        Server::create_channel(const std::string& name, const std::string& key, Client* client)
{
    Channel *channel = new Channel(name, key, client);
    _channels.push_back(channel);

    return channel;
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
