/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/24 20:58:45 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/18 21:12:09 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* Constructor and Destructor */

Server::Server(const std::string &port, const std::string &pass) : _port(port), _pass(pass)
{
    _running = 1;
    _sock = new_socket();
    
    _host = "127.0.0.1";
}

Server::~Server() {}

