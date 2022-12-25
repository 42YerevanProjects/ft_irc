/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shovsepy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/25 19:06:42 by shovsepy          #+#    #+#             */
/*   Updated: 2022/12/25 19:15:51 by shovsepy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* Constructor and Destructor */

Client::Client(int, fd, int, port, const std::string &hostname)
    : _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE)
{

}


Client::~Client()
{

}


/* Getters */

//TODO: to be implemented
